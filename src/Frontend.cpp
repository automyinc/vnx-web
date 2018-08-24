
#include <vnx/web/Frontend.h>
#include <vnx/web/StreamRead.hxx>
#include <vnx/InternalThread.h>

#include <mutex>
#include <condition_variable>

#include <queue>
#include <unordered_map>
#include <unordered_set>

#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/epoll.h>


namespace vnx {
namespace web {

void set_non_block(int fd) {
	if(::fcntl(fd, F_SETFL, ::fcntl(fd, F_GETFL) | O_NONBLOCK) != 0) {
		throw std::runtime_error("fcntl(F_SETFL, O_NONBLOCK) failed with: " + std::to_string(errno));
	}
}


class Frontend::AcceptLoop : public InternalThread {
public:
	std::shared_ptr<InternalThread> poll_loop;
	
	int server_sock = -1;
	std::shared_ptr<const Endpoint> endpoint;
	
	InternalPipe<int> new_sockets;
	
protected:
	void main() override {
		
		pthread_setname_np(pthread_self(), "AcceptLoop");
		
		while(do_run) {
			const int sock = ::accept(server_sock, 0, 0);
			if(sock >= 0) {
				const int value = 1;
				::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
				new_sockets.push(sock);
				poll_loop->notify();
			} else {
				switch(errno) {
					case EMFILE:
					case ENFILE:
					case ENOBUFS:
					case ENOMEM:
						vnx::log_warn().out << "Frontend: accept() failed with: " << errno;
						::usleep(500*1000);
						break;
					default:
						do_run = false;
				}
			}
		}
		
		::close(server_sock);
		
		poll_loop = 0;
	}
	
};


class Frontend::PollLoop : public InternalThread {
public:
	Frontend* frontend = 0;
	std::shared_ptr<AcceptLoop> accept_loop;
	std::shared_ptr<ReadLoop> read_loop;
	
	int max_num_events = 64 * 1024;
	
	InternalPipe<std::pair<Hash128, int>> new_read_avail;
	
	void notify() override {
		std::lock_guard<std::mutex> lock(mutex);
		if(notify_pipe[1] >= 0) {
			const char data = 0;
			const ssize_t res = ::write(notify_pipe[1], &data, 1);
		}
	}

protected:
	struct stream_state_t {
		Hash128 id;
		int64_t resume_time = -1;
		int64_t last_write_time = -1;
		int sock = -1;
		bool is_blocked = false;
	};
	
	struct write_state_t {
		std::queue<std::shared_ptr<const StreamWrite>> samples;
		size_t chunk_index = 0;
		size_t chunk_offset = 0;
		size_t vector_index = 0;
	};
	
	int epoll_fd = -1;
	int notify_pipe[2] = {-1, -1};
	std::unordered_map<Hash128, size_t> stream_map;
	std::vector<stream_state_t> state_vector;
	std::vector<epoll_event> new_events;
	std::vector<size_t> free_list;
	
	size_t write_block_size = 0;
	int64_t last_timeout_check_time = -1;
	std::unordered_map<Hash128, write_state_t> write_set;
	
	Hash128 add_stream(int sock, uint32_t events) {
		size_t index;
		if(free_list.empty()) {
			index = state_vector.size();
			state_vector.resize(index + 1);
		} else {
			index = free_list.back();
			free_list.pop_back();
		}
		
		stream_state_t& state = state_vector[index];
		state = stream_state_t();
		state.id = Hash128::rand();
		state.sock = sock;
		stream_map[state.id] = index;
		
		epoll_event event = {};
		event.events = events;
		event.data.u64 = index;
		::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event);
		
		frontend->num_connections++;
		frontend->num_accept++;
		return state.id;
	}
	
	void remove_stream(size_t index) {
		stream_state_t& state = state_vector[index];
		::epoll_ctl(epoll_fd, EPOLL_CTL_DEL, state.sock, 0);
		stream_map.erase(state.id);
		write_set.erase(state.id);
		state = stream_state_t();
		free_list.push_back(index);
		frontend->num_connections--;
		frontend->num_close++;
	}
	
	void init() override {
		std::lock_guard<std::mutex> lock(mutex);
		if(::pipe(notify_pipe) != 0) {
			throw std::runtime_error("pipe() failed with: " + std::to_string(errno));
		}
		set_non_block(notify_pipe[0]);
		set_non_block(notify_pipe[1]);
	}
	
	void main() override {
		
		pthread_setname_np(pthread_self(), "PollLoop");
		
		write_block_size = size_t(frontend->write_block_size > 0 ? frontend->write_block_size : 4096);
		
		epoll_fd = ::epoll_create(max_num_events);
		if(epoll_fd < 0) {
			log_error().out << "Frontend: epoll_create() failed with " << errno;
			return;
		}
		new_events.resize(max_num_events);
		
		add_stream(notify_pipe[0], EPOLLIN);
		
		while(do_run) {
			loop();
		}
		
		auto events = StreamEventArray::create();
		for(stream_state_t& state : state_vector) {
			if(state.sock >= 0) {
				events->array.push_back(stream_event_t::create(state.id, stream_event_t::EVENT_EOF));
				::close(state.sock);
			}
		}
		publish(events, frontend->output);
		
		accept_loop = 0;
		read_loop = 0;
		
		{
			std::lock_guard<std::mutex> lock(mutex);
			::close(notify_pipe[0]);
			::close(notify_pipe[1]);
			notify_pipe[0] = -1;
			notify_pipe[1] = -1;
		}
	}
	
	void loop();
	
};


class Frontend::ReadLoop : public InternalThread {
public:
	Frontend* frontend = 0;
	std::shared_ptr<PollLoop> poll_loop;
	
	InternalPipe<Hash128> set_poll_in;
	InternalPipe<Hash128> close_stream;
	
protected:
	size_t read_block_size = 0;
	std::unordered_map<Hash128, int> read_set;
	
	void main() override {
		
		pthread_setname_np(pthread_self(), "ReadLoop");
		
		read_block_size = size_t(frontend->read_block_size > 0 ? frontend->read_block_size : 4096);
		
		while(do_run) {
			loop();
		}
		
		poll_loop = 0;
	}
	
	void loop() {
		bool do_notify = false;
		{
			auto input = poll_loop->new_read_avail.pop_all();
			for(const std::pair<Hash128, int>& entry : *input) {
				read_set[entry.first] = entry.second;
			}
		}
		{
			std::vector<Hash128> remove_list;
			std::shared_ptr<StreamRead> sample;
			for(const auto& entry : read_set) {
				if(!sample) {
					sample = StreamRead::create();
					sample->channel = frontend->channel;
					sample->data.reserve(read_block_size);
				}
				const ssize_t num_read = ::read(entry.second, sample->data.data(), read_block_size);
				if(num_read > 0) {
					sample->stream = entry.first;
					sample->data.set_size(size_t(num_read));
					publish(sample, frontend->output, BLOCKING);	// publish needs to be blocking since we cannot drop any samples
					sample = 0;
					frontend->num_bytes_read += num_read;
				}
				if(num_read < ssize_t(read_block_size)) {
					remove_list.push_back(entry.first);
					if(num_read != 0) {
						set_poll_in.push(entry.first);
						do_notify = true;
					} else {
						close_stream.push(entry.first);
						do_notify = true;
					}
				}
			}
			for(const Hash128& id : remove_list) {
				read_set.erase(id);
			}
		}
		if(do_notify) {
			poll_loop->notify();
		}
		if(read_set.empty()) {
			wait();
		}
	}
	
};


void Frontend::PollLoop::loop() {
	const int64_t now = vnx::get_time_millis();
	std::shared_ptr<StreamEventArray> stream_events = StreamEventArray::create();
	std::vector<size_t> close_list;
	{
		auto input = accept_loop->new_sockets.pop_all();
		for(const int sock : *input) {
			set_non_block(sock);
			const Hash128 id = add_stream(sock, EPOLLIN | EPOLLONESHOT);
			stream_events->array.push_back(stream_event_t::create(id, stream_event_t::EVENT_NEW));
		}
		if(!do_run) {
			return;
		}
	}
	{
		auto input = read_loop->set_poll_in.pop_all();
		for(const Hash128& id : *input) {
			auto iter = stream_map.find(id);
			if(iter != stream_map.end()) {
				const stream_state_t& state = state_vector[iter->second];
				epoll_event event = {};
				if(state.is_blocked) {
					event.events = EPOLLIN | EPOLLOUT | EPOLLONESHOT;
				} else {
					event.events = EPOLLIN | EPOLLONESHOT;
				}
				event.data.u64 = iter->second;
				::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, state.sock, &event);
			}
		}
	}
	{
		auto input = read_loop->close_stream.pop_all();
		for(const Hash128& id : *input) {
			auto iter = stream_map.find(id);
			if(iter != stream_map.end()) {
				close_list.push_back(iter->second);
			}
		}
	}
	{
		auto input = frontend->new_event_list.pop_all();
		for(const stream_event_t& event : *input) {
			auto iter = stream_map.find(event.stream);
			if(iter == stream_map.end()) {
				continue;
			}
			stream_state_t& state = state_vector[iter->second];
			switch(event.event) {
				case stream_event_t::EVENT_CLOSE:
					if(state.sock >= 0) {
						close_list.push_back(iter->second);
					}
					break;
			}
		}
	}
	{
		auto input = frontend->new_write_data.pop_all();
		for(const std::shared_ptr<const StreamWrite>& sample : *input) {
			auto iter = stream_map.find(sample->stream);
			if(iter == stream_map.end()) {
				continue;
			}
			write_state_t& state = write_set[sample->stream];
			state.vector_index = iter->second;
			state.samples.push(sample);
		}
	}
	{
		std::vector<Hash128> done_list;
		for(auto& entry : write_set) {
			write_state_t& state = entry.second;
			stream_state_t& stream = state_vector[state.vector_index];
			if(stream.is_blocked) {
				continue;
			}
			if(state.samples.empty()) {
				done_list.push_back(entry.first);
				continue;
			}
			size_t total_written = 0;
			std::shared_ptr<const StreamWrite> sample = state.samples.front();
			while(state.chunk_index < sample->chunks.size()) {
				const vnx::Buffer& buffer = sample->chunks[state.chunk_index];
				if(buffer.size() > state.chunk_offset) {
					const ssize_t num_left = buffer.size() - state.chunk_offset;
					const ssize_t num_to_write = num_left > write_block_size ? write_block_size : num_left;
					const ssize_t num_written = ::send(stream.sock, buffer.data(state.chunk_offset), size_t(num_to_write), MSG_NOSIGNAL);
					if(num_written > 0) {
						total_written += num_written;
						stream.last_write_time = now;
						frontend->num_bytes_written += num_written;
					}
					if(num_written == num_left) {
						state.chunk_index++;
						state.chunk_offset = 0;
					} else if(num_written == num_to_write) {
						state.chunk_offset += num_written;
					} else {
						stream.is_blocked = true;
						if(num_written > 0) {
							state.chunk_offset += num_written;
						}
						epoll_event event = {};
						event.events = EPOLLOUT | EPOLLIN | EPOLLONESHOT;		// EPOLLIN to detect disconnect
						event.data.u64 = state.vector_index;
						::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, stream.sock, &event);
						break;
					}
					if(total_written >= write_block_size) {
						break;
					}
				} else {
					state.chunk_index++;
					state.chunk_offset = 0;
				}
			}
			if(state.chunk_index >= sample->chunks.size()) {
				state.chunk_index = 0;
				state.samples.pop();
				if(state.samples.empty()) {
					done_list.push_back(entry.first);
					if(sample->is_eof) {
						close_list.push_back(state.vector_index);	// server decided to close connection
					}
				}
			}
		}
		for(const Hash128& id : done_list) {
			write_set.erase(id);
		}
	}
	if(now - last_timeout_check_time > frontend->connection_timeout_ms / 5) {
		for(const auto& entry : stream_map) {
			stream_state_t& state = state_vector[entry.second];
			if(state.sock != notify_pipe[0]) {
				if(state.last_write_time < 0) {
					state.last_write_time = now;
				}
				if(now - state.last_write_time > frontend->connection_timeout_ms) {
					close_list.push_back(entry.second);		// connection timeout
					frontend->num_timeout++;
				}
			}
		}
		last_timeout_check_time = now;
	}
	{
		for(const size_t index : close_list) {
			stream_state_t& state = state_vector[index];
			if(state.sock >= 0) {
				stream_events->array.push_back(stream_event_t::create(state.id, stream_event_t::EVENT_EOF));
				const int res = ::close(state.sock);
				remove_stream(index);
			}
		}
	}
	
	if(!stream_events->array.empty()) {
		publish(stream_events, frontend->output);
	}
	
	const int num_events = ::epoll_wait(epoll_fd, new_events.data(), new_events.size(), 100);
	if(num_events < 0) {
		log_error().out << "Frontend: epoll_wait() failed with " << errno;
		do_run = false;
		return;
	}
	
	{
		bool do_notify = false;
		for(int i = 0; i < num_events; ++i) {
			epoll_event& event = new_events[i];
			stream_state_t& state = state_vector[event.data.u64];
			if(state.sock < 0) {
				continue;
			}
			if(state.sock == notify_pipe[0]) {
				char buf[4096];
				while(::read(state.sock, buf, sizeof(buf)) == sizeof(buf));
				continue;
			}
			if(event.events & EPOLLOUT) {
				state.is_blocked = false;
			}
			if(event.events & EPOLLIN) {
				if(!state.is_blocked) {
					new_read_avail.push(std::make_pair(state.id, state.sock));
					do_notify = true;
				}
			}
			if(event.events & EPOLLOUT) {
				event.events = EPOLLIN | EPOLLONESHOT;
				::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, state.sock, &event);
			}
		}
		if(do_notify) {
			read_loop->notify();
		}
	}
	frontend->num_poll_iter++;
}


Frontend::Frontend(const std::string& _vnx_name)
	:	FrontendBase(_vnx_name)
{
}

void Frontend::main() {
	
	endpoint = vnx::clone(Endpoint::from_url(address));
	endpoint->listen_queue_size = listen_queue_size;
	
	setup_timer = set_timer_millis(1000, std::bind(&Frontend::setup, this));
	
	setup();
	
	Super::main();
	
	if(server_sock >= 0) {
		::shutdown(server_sock, SHUT_RDWR);
	}
	if(accept_loop) {
		accept_loop->close();
	}
	if(poll_loop) {
		poll_loop->close();
	}
	if(read_loop) {
		read_loop->close();
	}
}

void Frontend::setup() {
	try {
		if(server_sock < 0) {
			server_sock = endpoint->open();
		}
		endpoint->bind(server_sock);
		endpoint->listen(server_sock);
	} catch(std::exception& ex) {
		log(WARN).out << ex.what();
		return;
	}
	setup_timer->stop();
	
	accept_loop = std::make_shared<AcceptLoop>();
	poll_loop = std::make_shared<PollLoop>();
	read_loop = std::make_shared<ReadLoop>();
	
	accept_loop->poll_loop = poll_loop;
	accept_loop->server_sock = server_sock;
	accept_loop->endpoint = endpoint;
	
	poll_loop->frontend = this;
	poll_loop->accept_loop = accept_loop;
	poll_loop->read_loop = read_loop;
	
	read_loop->frontend = this;
	read_loop->poll_loop = poll_loop;
	
	accept_loop->start();
	poll_loop->start();
	read_loop->start();
	
	subscribe(input, 1000);
	subscribe(channel);
	
	set_timer_millis(1000, std::bind(&Frontend::update, this));
	
	log(INFO).out << "running on " << endpoint->to_url();
}

void Frontend::handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) {
	new_event_list.insert(value->array.begin(), value->array.end());
	poll_loop->notify();
}

void Frontend::handle(std::shared_ptr<const ::vnx::web::StreamWrite> value) {
	new_write_data.push(value);
	poll_loop->notify();
}

void Frontend::update() {
	log(INFO).out << "sockets=" << (num_connections - 1) << ", accept=" << num_accept << "/s, timeout="
			<< num_timeout << "/s, close=" << num_close << "/s, poll=" << num_poll_iter
			<< "/s, read=" << (float(num_bytes_read / 1024) / 1024) << " MB/s, write="
			<< (float(num_bytes_written / 1024) / 1024) << " MB/s";
	num_accept = 0;
	num_bytes_read = 0;
	num_bytes_written = 0;
	num_timeout = 0;
	num_close = 0;
	num_poll_iter = 0;
}


} // web
} // vnx
