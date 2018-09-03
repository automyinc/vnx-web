
#include <vnx/web/Frontend.h>
#include <vnx/web/StreamRead.hxx>
#include <vnx/web/StreamWrite.hxx>
#include <vnx/web/StreamEventArray.hxx>
#include <vnx/Thread.h>
#include <vnx/Pipe.h>

#include <mutex>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include <string.h>
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
		throw std::runtime_error("fcntl(F_SETFL, O_NONBLOCK) failed with: " + std::string(::strerror(errno)));
	}
}

class NewSocket : public vnx::Message {
public:
	int sock = -1;
};

class NewReadAvail : public vnx::Message {
public:
	std::vector<std::pair<Hash128, int>> list;
};

class SetPollIn : public vnx::Message {
public:
	std::vector<vnx::Hash128> list;
};

class CloseStream : public vnx::Message {
public:
	std::vector<vnx::Hash128> list;
};


class Frontend::AcceptLoop : public Thread {
public:
	int server_sock = -1;
	
	std::shared_ptr<Pipe> new_sockets = Pipe::create();		// output
	
	AcceptLoop() : Thread("AcceptLoop") {}
	
protected:
	void main() override {
		
		while(vnx_do_run) {
			auto msg = std::make_shared<NewSocket>();
			msg->sock = ::accept(server_sock, 0, 0);
			if(msg->sock >= 0) {
				const int value = 1;
				::setsockopt(msg->sock, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
				vnx::send_msg(new_sockets, msg, BLOCKING);
			} else {
				switch(errno) {
					case EMFILE:
					case ENFILE:
					case ENOBUFS:
					case ENOMEM:
						vnx::log_warn().out << "Frontend: accept() failed with: " << ::strerror(errno);
						::usleep(500*1000);
						break;
					default:
						exit();
				}
			}
		}
		::close(server_sock);
	}
	
};


class Frontend::PollLoop : public Thread {
public:
	Frontend* frontend = 0;
	int max_num_events = 16 * 1024;
	
	std::shared_ptr<Pipe> new_sockets;			// input
	std::shared_ptr<Pipe> set_poll_in;			// input
	std::shared_ptr<Pipe> close_stream;			// input
	
	std::shared_ptr<Pipe> new_read_avail = Pipe::create();		// output
	
	PollLoop() : Thread("PollLoop") {}
	
	void notify(std::shared_ptr<Pipe> pipe) override {
		Thread::notify(pipe);
		{
			std::lock_guard<std::mutex> lock(vnx_mutex);
			const int pipe = notify_pipe[1];
			if(pipe >= 0) {
				const char data = 0;
				const ssize_t res = ::write(pipe, &data, 1);
			}
		}
	}

protected:
	struct stream_state_t {
		Hash128 id;
		int64_t last_write_time = -1;
		int sock = -1;
		bool is_internal = false;
		bool is_blocked = false;
	};
	
	struct write_state_t {
		std::queue<std::shared_ptr<const StreamWrite>> samples;
		const vnx::Memory::chunk_t* chunk = 0;
		size_t chunk_offset = 0;
		size_t vector_index = 0;
		int64_t backlog = 0;
	};
	
	int epoll_sock = -1;
	int notify_pipe[2] = {-1, -1};
	size_t write_block_size = 0;
	int64_t last_timeout_check_time = -1;
	std::unordered_map<Hash128, size_t> stream_map;
	std::unordered_map<Hash128, write_state_t> write_set;
	std::vector<stream_state_t> state_vector;
	std::vector<epoll_event> new_events;
	std::vector<size_t> free_list;
	
	std::shared_ptr<Pipe> input_pipe;
	
	void init() override {
		if(::pipe(notify_pipe) != 0) {
			throw std::runtime_error("pipe() failed with: " + std::string(::strerror(errno)));
		}
		set_non_block(notify_pipe[0]);
		set_non_block(notify_pipe[1]);
		
		vnx::connect(new_sockets, this);
		vnx::connect(set_poll_in, this);
		vnx::connect(close_stream, this);
	}
	
	void main() override {
		
		input_pipe = subscribe(frontend->input);
		subscribe(frontend->channel);
		
		write_block_size = size_t(frontend->write_block_size > 0 ? frontend->write_block_size : 4096);
		
		epoll_sock = ::epoll_create(max_num_events);
		if(epoll_sock < 0) {
			log_error().out << "Frontend: epoll_create() failed with " << errno;
			return;
		}
		new_events.resize(max_num_events);
		
		add_stream(notify_pipe[0], EPOLLIN, true);
		
		while(vnx_do_run) {
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
		
		::close(epoll_sock);
		
		{
			std::lock_guard<std::mutex> lock(vnx_mutex);
			::close(notify_pipe[0]);
			::close(notify_pipe[1]);
			notify_pipe[0] = -1;
			notify_pipe[1] = -1;
		}
	}
	
	Hash128 add_stream(int sock, uint32_t events, bool is_internal = false) {
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
		state.is_internal = is_internal;
		stream_map[state.id] = index;
		
		epoll_event event = {};
		event.events = events;
		event.data.u64 = index;
		::epoll_ctl(epoll_sock, EPOLL_CTL_ADD, sock, &event);
		
		frontend->num_connections++;
		frontend->num_accept++;
		return state.id;
	}
	
	void remove_stream(size_t index) {
		stream_state_t& state = state_vector[index];
		::epoll_ctl(epoll_sock, EPOLL_CTL_DEL, state.sock, 0);
		stream_map.erase(state.id);
		write_set.erase(state.id);
		state = stream_state_t();
		free_list.push_back(index);
		frontend->num_connections--;
		frontend->num_close++;
	}
	
	void loop();
	
};


class Frontend::ReadLoop : public Thread {
public:
	Frontend* frontend = 0;
	
	std::shared_ptr<Pipe> new_read_avail;		// input
	
	std::shared_ptr<Pipe> set_poll_in = Pipe::create();		// output
	std::shared_ptr<Pipe> close_stream = Pipe::create();	// output
	
	ReadLoop() : Thread("ReadLoop") {}
	
protected:
	size_t read_block_size = 0;
	std::unordered_map<Hash128, int> read_set;
	
	void init() override {
		vnx::connect(new_read_avail, this, 0);		// queue needs to be unlimited since we dont want to block poll loop
	}
	
	void main() override {
		
		read_block_size = size_t(frontend->read_block_size > 0 ? frontend->read_block_size : 4096);
		
		while(vnx_do_run) {
			loop();
		}
	}
	
	void loop() {
		while(auto msg = (read_set.empty() ? Thread::read_blocking() : Thread::read())) {
			auto sample = std::dynamic_pointer_cast<const NewReadAvail>(msg);
			if(sample) {
				for(const std::pair<Hash128, int>& entry : sample->list) {
					read_set[entry.first] = entry.second;
				}
			}
		}
		
		std::vector<Hash128> done_list;
		std::shared_ptr<StreamRead> sample;
		auto set_poll_in_msg = std::make_shared<SetPollIn>();
		auto close_stream_msg = std::make_shared<CloseStream>();
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
				done_list.push_back(entry.first);
				if(num_read != 0) {
					set_poll_in_msg->list.push_back(entry.first);
				} else {
					close_stream_msg->list.push_back(entry.first);
				}
			}
		}
		for(const Hash128& id : done_list) {
			read_set.erase(id);
		}
		if(!set_poll_in_msg->list.empty()) {
			vnx::send_msg(set_poll_in, set_poll_in_msg, BLOCKING);
		}
		if(!close_stream_msg->list.empty()) {
			vnx::send_msg(close_stream, close_stream_msg, BLOCKING);
		}
	}
	
};


void Frontend::PollLoop::loop() {
	
	const int64_t now = vnx::get_time_millis();
	std::vector<size_t> close_list;
	auto stream_events = StreamEventArray::create();
	
	while(auto msg = Thread::read()) {
		{
			auto sample = std::dynamic_pointer_cast<const SetPollIn>(msg);
			if(sample) {
				for(const Hash128& id : sample->list) {
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
						::epoll_ctl(epoll_sock, EPOLL_CTL_MOD, state.sock, &event);
					}
				}
				continue;
			}
		}
		{
			auto sample = std::dynamic_pointer_cast<const NewSocket>(msg);
			if(sample) {
				set_non_block(sample->sock);
				const Hash128 id = add_stream(sample->sock, EPOLLIN | EPOLLONESHOT);
				stream_events->array.push_back(stream_event_t::create(id, stream_event_t::EVENT_NEW));
				continue;
			}
		}
		{
			auto sample = std::dynamic_pointer_cast<const CloseStream>(msg);
			if(sample) {
				for(const Hash128& id : sample->list) {
					auto iter = stream_map.find(id);
					if(iter != stream_map.end()) {
						close_list.push_back(iter->second);
					}
				}
				continue;
			}
		}
		auto sample = std::dynamic_pointer_cast<const vnx::Sample>(msg);
		if(sample) {
			{
				auto value = std::dynamic_pointer_cast<const StreamWrite>(sample->value);
				if(value) {
					auto iter = stream_map.find(value->stream);
					if(iter == stream_map.end()) {
						continue;
					}
					write_state_t& state = write_set[value->stream];
					state.vector_index = iter->second;
					state.samples.push(value);
					state.backlog += value->get_size();
					continue;
				}
			}
			{
				auto value = std::dynamic_pointer_cast<const StreamEventArray>(sample->value);
				if(value) {
					for(const stream_event_t& event : value->array) {
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
					continue;
				}
			}
		}
	}
	{
		int64_t write_backlog = 0;
		std::vector<Hash128> done_list;
		for(auto& entry : write_set) {
			write_state_t& state = entry.second;
			stream_state_t& stream = state_vector[state.vector_index];
			write_backlog += state.backlog;
			if(stream.is_blocked) {
				continue;
			}
			if(state.samples.empty()) {
				done_list.push_back(entry.first);
				continue;
			}
			int64_t total_written = 0;
			auto sample = state.samples.front();
			if(!state.chunk) {
				state.chunk = sample->data.front();
			}
			while(state.chunk) {
				if(state.chunk->size() > state.chunk_offset) {
					const ssize_t num_left = state.chunk->size() - state.chunk_offset;
					const ssize_t num_to_write = num_left > write_block_size ? write_block_size : num_left;
					const ssize_t num_written = ::send(stream.sock, state.chunk->data() + state.chunk_offset, size_t(num_to_write), MSG_NOSIGNAL);
					if(num_written > 0) {
						total_written += num_written;
					}
					if(num_written == num_left) {
						state.chunk = state.chunk->next();
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
						::epoll_ctl(epoll_sock, EPOLL_CTL_MOD, stream.sock, &event);
						break;
					}
					if(total_written >= write_block_size) {
						break;
					}
				} else {
					state.chunk = state.chunk->next();
					state.chunk_offset = 0;
				}
			}
			if(total_written > 0) {
				stream.last_write_time = now;
				state.backlog -= total_written;
				write_backlog -= total_written;
				frontend->num_bytes_written += total_written;
			}
			if(state.chunk == 0) {
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
		if(write_backlog > frontend->max_write_backlog) {
			if(!input_pipe->get_is_paused()) {
				vnx::log_warn().out << "Blocked input: write_backlog=" << write_backlog;
			}
			input_pipe->pause();
		}
		if(write_backlog <= frontend->max_write_backlog / 2) {
			if(input_pipe->get_is_paused()) {
				vnx::log_warn().out << "Resumed input: write_backlog=" << write_backlog;
			}
			input_pipe->resume();
		}
	}
	if(now - last_timeout_check_time > frontend->connection_timeout_ms / 5) {
		for(const auto& entry : stream_map) {
			stream_state_t& state = state_vector[entry.second];
			if(!state.is_internal) {
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
	
	const int num_events = ::epoll_wait(epoll_sock, new_events.data(), new_events.size(), 100);
	if(num_events < 0) {
		if(errno != EINTR) {
			log_error().out << "Frontend: epoll_wait() failed with " << errno;
		}
		exit();
		return;
	}
	
	{
		auto new_read_avail_msg = std::make_shared<NewReadAvail>();
		for(int i = 0; i < num_events; ++i) {
			epoll_event& event = new_events[i];
			stream_state_t& state = state_vector[event.data.u64];
			if(state.sock < 0) {
				continue;
			}
			if(state.is_internal) {
				if(state.sock == notify_pipe[0]) {
					char buf[4096];
					while(::read(state.sock, buf, sizeof(buf)) == sizeof(buf));
				}
				continue;
			}
			if(event.events & EPOLLOUT) {
				state.is_blocked = false;
			}
			if(event.events & EPOLLIN) {
				new_read_avail_msg->list.push_back(std::make_pair(state.id, state.sock));
			} else if(event.events & EPOLLOUT) {
				event.events = EPOLLIN | EPOLLONESHOT;
				::epoll_ctl(epoll_sock, EPOLL_CTL_MOD, state.sock, &event);
			}
		}
		if(!new_read_avail_msg->list.empty()) {
			vnx::send_msg(new_read_avail, new_read_avail_msg, BLOCKING);
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
		accept_loop->stop();
	}
	if(poll_loop) {
		poll_loop->stop();
	}
	if(read_loop) {
		read_loop->stop();
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
	
	accept_loop->server_sock = server_sock;
	
	poll_loop->frontend = this;
	poll_loop->new_sockets = accept_loop->new_sockets;
	poll_loop->set_poll_in = read_loop->set_poll_in;
	poll_loop->close_stream = read_loop->close_stream;
	
	read_loop->frontend = this;
	read_loop->new_read_avail = poll_loop->new_read_avail;
	
	read_loop->start();
	poll_loop->start();
	accept_loop->start();
	
	set_timer_millis(1000, std::bind(&Frontend::update, this));
	
	log(INFO).out << "running on " << endpoint->to_url();
}

void Frontend::update() {
	log(INFO).out << "sockets=" << (num_connections - 1) << ", accept=" << num_accept << "/s, timeout="
			<< num_timeout << "/s, close=" << num_close << "/s, poll=" << num_poll_iter
			<< "/s, read=" << (float(num_bytes_read) / 1024 / 1024) << " MB/s, write="
			<< (float(num_bytes_written) / 1024 / 1024) << " MB/s";
	num_accept = 0;
	num_bytes_read = 0;
	num_bytes_written = 0;
	num_timeout = 0;
	num_close = 0;
	num_poll_iter = 0;
}


} // web
} // vnx
