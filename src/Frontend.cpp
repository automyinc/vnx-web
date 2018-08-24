
#include <vnx/web/Frontend.h>
#include <vnx/web/StreamRead.hxx>

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


class Loop : protected Publisher {
public:
	virtual ~Loop() {
		close();
	}
	
	void start() {
		thread = std::thread(&Loop::entry, this);
	}
	
	void stop() {
		{
			std::lock_guard<std::mutex> lock(mutex);
			do_run = false;
		}
		notify();
	}
	
	void join() {
		if(thread.joinable()) {
			thread.join();
		}
	}
	
	void close() {
		stop();
		join();
	}
	
	virtual void notify() {
		std::lock_guard<std::mutex> lock(mutex);
		notify_counter++;
		condition.notify_all();
	}
	
protected:
	std::mutex mutex;
	
	bool do_run = true;
	
	virtual void init() {}
	
	virtual void main() = 0;
	
	void wait() {
		std::unique_lock<std::mutex> lock(mutex);
		while(consume_count == notify_counter) {
			condition.wait(lock);
		}
		consume_count = notify_counter;
	}
	
private:
	void entry() {
		init();
		main();
	}
	
private:
	std::thread thread;
	std::condition_variable condition;
	uint64_t notify_counter = 0;
	uint64_t consume_count = 0;
	
};


class Frontend::AcceptLoop : public Loop {
public:
	std::shared_ptr<Loop> poll_loop;
	
	int server_sock = -1;
	std::shared_ptr<const Endpoint> endpoint;
	
	DoubleBuffer<std::vector<int>> new_sockets;
	
protected:
	void main() override {
		
		pthread_setname_np(pthread_self(), "AcceptLoop");
		
		while(do_run) {
			const int sock = ::accept(server_sock, 0, 0);
			if(sock >= 0) {
				const int value = 1;
				::setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
				add_socket(sock);
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
	
	void add_socket(int sock) {
		if(new_sockets.write_lock()) {
			new_sockets.input().clear();
		}
		new_sockets.input().push_back(sock);
		new_sockets.write_unlock();
		poll_loop->notify();
	}
	
};


class Frontend::PollLoop : public Loop {
public:
	Frontend* frontend = 0;
	std::shared_ptr<AcceptLoop> accept_loop;
	std::shared_ptr<ReadLoop> read_loop;
	std::shared_ptr<WriteLoop> write_loop;
	
	int max_num_events = 64 * 1024;
	
	DoubleBuffer<std::vector<std::pair<Hash128, int>>> new_read_avail;
	DoubleBuffer<std::vector<std::pair<Hash128, int>>> new_write_avail;
	DoubleBuffer<std::vector<Hash128>> remove_read_avail;
	DoubleBuffer<std::vector<Hash128>> remove_write_avail;
	
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
		int sock = -1;
		bool is_paused = false;
	};
	
	int epoll_fd = -1;
	int notify_pipe[2] = {-1, -1};
	std::unordered_map<Hash128, size_t> stream_map;
	std::vector<stream_state_t> state_vector;
	std::unordered_set<size_t> pause_set;
	std::vector<epoll_event> new_events;
	std::vector<size_t> free_list;
	
	Hash128 add_stream(int sock) {
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
		event.events = EPOLLIN | EPOLLONESHOT;
		event.data.u64 = index;
		::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event);
		
		frontend->num_connections++;
		frontend->num_accept++;
		return state.id;
	}
	
	void remove_stream(size_t index) {
		stream_state_t& state = state_vector[index];
		stream_map.erase(state.id);
		::epoll_ctl(epoll_fd, EPOLL_CTL_DEL, state.sock, 0);
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
		
		epoll_fd = ::epoll_create(max_num_events);
		if(epoll_fd < 0) {
			log_error().out << "Frontend: epoll_create() failed with " << errno;
			return;
		}
		new_events.resize(max_num_events);
		
		add_stream(notify_pipe[0]);
		
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
		write_loop = 0;
		
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


class Frontend::ReadLoop : public Loop {
public:
	Frontend* frontend = 0;
	std::shared_ptr<PollLoop> poll_loop;
	
	DoubleBuffer<std::vector<Hash128>> set_poll_in;
	DoubleBuffer<std::vector<Hash128>> close_stream;
	
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
			const std::vector<std::pair<Hash128, int>>* input = poll_loop->new_read_avail.try_read_lock();
			if(input) {
				for(const std::pair<Hash128, int>& entry : *input) {
					read_set[entry.first] = entry.second;
				}
				poll_loop->new_read_avail.read_unlock();
			}
		}
		{
			const std::vector<Hash128>* input = poll_loop->remove_read_avail.try_read_lock();
			if(input) {
				for(const Hash128& id : *input) {
					read_set.erase(id);
				}
				poll_loop->remove_read_avail.read_unlock();
			}
		}
		{
			if(set_poll_in.write_lock()) {
				set_poll_in.input().clear();
			}
			if(close_stream.write_lock()) {
				close_stream.input().clear();
			}
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
					publish(sample, frontend->output);
					sample = 0;
					frontend->num_bytes_read += num_read;
				}
				if(num_read < ssize_t(read_block_size)) {
					remove_list.push_back(entry.first);
					if(num_read != 0) {
						set_poll_in.input().push_back(entry.first);
					} else {
						close_stream.input().push_back(entry.first);
					}
				}
			}
			for(const Hash128& id : remove_list) {
				read_set.erase(id);
			}
			do_notify = !set_poll_in.input().empty() || !close_stream.input().empty();
			set_poll_in.write_unlock();
			close_stream.write_unlock();
		}
		if(do_notify) {
			poll_loop->notify();
		}
		if(read_set.empty()) {
			wait();
		}
	}
	
};


class Frontend::WriteLoop : public Loop {
public:
	Frontend* frontend = 0;
	std::shared_ptr<PollLoop> poll_loop;
	
	DoubleBuffer<std::vector<Hash128>> set_poll_out;
	DoubleBuffer<std::vector<Hash128>> close_stream;
	
protected:
	struct stream_state_t {
		int64_t last_write_time = 0;
		int sock = -1;
	};
	
	struct write_state_t {
		std::queue<std::shared_ptr<const StreamWrite>> samples;
		size_t index = 0;
		size_t offset = 0;
		int64_t last_write_time = 0;
		int sock = -1;
	};
	
	int64_t last_update_time = 0;
	size_t write_block_size = 0;
	std::unordered_map<Hash128, stream_state_t> stream_map;
	std::unordered_map<Hash128, write_state_t> write_set;
	
	void main() override {
		
		pthread_setname_np(pthread_self(), "WriteLoop");
		
		write_block_size = size_t(frontend->write_block_size > 0 ? frontend->write_block_size : 4096);
		
		while(do_run) {
			loop();
		}
		
		poll_loop = 0;
	}
	
	void loop() {
		const int64_t now = vnx::get_time_millis();
		bool do_notify = false;
		{
			const std::vector<std::pair<Hash128, int>>* input = poll_loop->new_write_avail.try_read_lock();
			if(input) {
				for(const std::pair<Hash128, int>& entry : *input) {
					stream_state_t& state = stream_map[entry.first];
					state.last_write_time = now;
					state.sock = entry.second;
					auto iter = write_set.find(entry.first);
					if(iter != write_set.end()) {
						iter->second.sock = entry.second;
					}
				}
				poll_loop->new_write_avail.read_unlock();
			}
		}
		{
			const std::vector<Hash128>* input = poll_loop->remove_write_avail.try_read_lock();
			if(input) {
				for(const Hash128& id : *input) {
					stream_map.erase(id);
					write_set.erase(id);
				}
				poll_loop->remove_write_avail.read_unlock();
			}
		}
		{
			const std::vector<std::shared_ptr<const StreamWrite>>* input = frontend->new_write_data.try_read_lock();
			if(input) {
				for(const std::shared_ptr<const StreamWrite>& sample : *input) {
					auto iter = stream_map.find(sample->stream);
					if(iter == stream_map.end()) {
						continue;
					}
					write_state_t& state = write_set[sample->stream];
					state.sock = iter->second.sock;
					state.last_write_time = now;
					state.samples.push(sample);
					iter->second.last_write_time = now;
				}
				frontend->new_write_data.read_unlock();
			}
		}
		{
			if(set_poll_out.write_lock()) {
				set_poll_out.input().clear();
			}
			if(close_stream.write_lock()) {
				close_stream.input().clear();
			}
			std::vector<Hash128> done_list;
			for(auto& entry : write_set) {
				write_state_t& state = entry.second;
				if(state.sock < 0) {
					continue;
				}
				if(state.samples.empty()) {
					done_list.push_back(entry.first);
					continue;
				}
				size_t total_written = 0;
				std::shared_ptr<const StreamWrite> sample = state.samples.front();
				while(state.index < sample->chunks.size()) {
					const vnx::Buffer& buffer = sample->chunks[state.index];
					if(buffer.size() > state.offset) {
						const ssize_t num_left = buffer.size() - state.offset;
						const ssize_t num_to_write = num_left > write_block_size ? write_block_size : num_left;
						const ssize_t num_written = ::send(state.sock, buffer.data(state.offset), size_t(num_to_write), MSG_NOSIGNAL);
						if(num_written > 0) {
							total_written += num_written;
							state.last_write_time = now;
							frontend->num_bytes_written += num_written;
						}
						if(num_written == num_left) {
							state.index++;
							state.offset = 0;
						} else if(num_written == num_to_write) {
							state.offset += num_written;
						} else {
							state.sock = -1;
							if(num_written > 0) {
								state.offset += num_written;
							}
							set_poll_out.input().push_back(entry.first);
							break;
						}
						if(total_written >= write_block_size) {
							break;
						}
					} else {
						state.index++;
					}
				}
				if(state.index >= sample->chunks.size()) {
					state.index = 0;
					state.samples.pop();
					if(state.samples.empty()) {
						done_list.push_back(entry.first);
						if(sample->is_eof) {
							close_stream.input().push_back(sample->stream);		// server decided to close connection
						}
					}
				}
			}
			for(const Hash128& id : done_list) {
				write_set.erase(id);
			}
			if(now - last_update_time > frontend->connection_timeout_ms / 5) {
				for(const auto& entry : stream_map) {
					if(now - entry.second.last_write_time > frontend->connection_timeout_ms) {
						bool is_timeout = true;
						auto iter = write_set.find(entry.first);
						if(iter != write_set.end()) {
							is_timeout = now - iter->second.last_write_time > frontend->connection_timeout_ms;
						}
						if(is_timeout) {
							close_stream.input().push_back(entry.first);		// connection timeout
							frontend->num_timeout++;
						}
					}
				}
				last_update_time = now;
			}
			do_notify = !set_poll_out.input().empty() || !close_stream.input().empty();
			set_poll_out.write_unlock();
			close_stream.write_unlock();
		}
		if(do_notify) {
			poll_loop->notify();
		}
		if(write_set.empty()) {
			wait();
		}
	}
	
};


void Frontend::PollLoop::loop() {
	const int64_t now = vnx::get_time_millis();
	std::shared_ptr<StreamEventArray> stream_events = StreamEventArray::create();
	std::vector<size_t> close_list;
	{
		const std::vector<int>* input = accept_loop->new_sockets.try_read_lock();
		if(input) {
			if(new_write_avail.write_lock()) {
				new_write_avail.input().clear();
			}
			for(const int sock : *input) {
				if(sock < 0) {
					do_run = false;
					break;
				}
				set_non_block(sock);
				const Hash128 id = add_stream(sock);
				new_write_avail.input().push_back(std::make_pair(id, sock));
				stream_events->array.push_back(stream_event_t::create(id, stream_event_t::EVENT_NEW));
			}
			new_write_avail.write_unlock();
			accept_loop->new_sockets.read_unlock();
		}
		if(!do_run) {
			return;
		}
	}
	{
		const std::vector<Hash128>* input = read_loop->set_poll_in.try_read_lock();
		if(input) {
			for(const Hash128& id : *input) {
				auto iter = stream_map.find(id);
				if(iter != stream_map.end()) {
					const stream_state_t& state = state_vector[iter->second];
					if(!state.is_paused) {
						epoll_event event = {};
						event.events = EPOLLIN | EPOLLONESHOT;
						event.data.u64 = iter->second;
						::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, state.sock, &event);
					}
				}
			}
			read_loop->set_poll_in.read_unlock();
		}
	}
	{
		const std::vector<Hash128>* input = write_loop->set_poll_out.try_read_lock();
		if(input) {
			if(remove_read_avail.write_lock()) {
				remove_read_avail.input().clear();
			}
			for(const Hash128& id : *input) {
				auto iter = stream_map.find(id);
				if(iter != stream_map.end()) {
					const stream_state_t& state = state_vector[iter->second];
					epoll_event event = {};
					event.events = EPOLLOUT | EPOLLONESHOT;
					event.data.u64 = iter->second;
					::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, state.sock, &event);
					remove_read_avail.input().push_back(state.id);
				}
			}
			remove_read_avail.write_unlock();
			write_loop->set_poll_out.read_unlock();
		}
	}
	{
		const std::vector<Hash128>* input = read_loop->close_stream.try_read_lock();
		if(input) {
			for(const Hash128& id : *input) {
				auto iter = stream_map.find(id);
				if(iter != stream_map.end()) {
					close_list.push_back(iter->second);
				}
			}
			read_loop->close_stream.read_unlock();
		}
	}
	{
		const std::vector<Hash128>* input = write_loop->close_stream.try_read_lock();
		if(input) {
			for(const Hash128& id : *input) {
				auto iter = stream_map.find(id);
				if(iter != stream_map.end()) {
					close_list.push_back(iter->second);
				}
			}
			write_loop->close_stream.read_unlock();
		}
	}
	{
		const std::vector<stream_event_t>* input = frontend->new_event_list.try_read_lock();
		if(input) {
			for(const stream_event_t& event : *input) {
				auto iter = stream_map.find(event.stream);
				if(iter == stream_map.end()) {
					continue;
				}
				stream_state_t& state = state_vector[iter->second];
				switch(event.event) {
					case stream_event_t::EVENT_PAUSE:
						state.resume_time = now + event.value;
						pause_set.insert(iter->second);
						break;
					case stream_event_t::EVENT_RESUME:
						state.resume_time = now;
						break;
					case stream_event_t::EVENT_CLOSE:
						if(state.sock >= 0) {
							close_list.push_back(iter->second);
						}
						break;
				}
			}
			frontend->new_event_list.read_unlock();
		}
	}
	{
		std::vector<size_t> remove_list;
		for(const size_t index : pause_set) {
			stream_state_t& state = state_vector[index];
			if(now < state.resume_time) {
				if(!state.is_paused) {
					state.is_paused = true;
					remove_read_avail.input().push_back(state.id);
				}
			} else {
				if(state.is_paused) {
					epoll_event event = {};
					event.events = EPOLLIN | EPOLLONESHOT;
					event.data.u64 = index;
					::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, state.sock, &event);
				}
				state.is_paused = false;
				remove_list.push_back(index);
			}
		}
		for(const size_t index : remove_list) {
			pause_set.erase(index);
		}
	}
	{
		if(remove_write_avail.write_lock()) {
			remove_write_avail.input().clear();
		}
		for(const size_t index : close_list) {
			stream_state_t& state = state_vector[index];
			remove_write_avail.input().push_back(state.id);
			stream_events->array.push_back(stream_event_t::create(state.id, stream_event_t::EVENT_EOF));
			const int res = ::close(state.sock);
			remove_stream(index);
		}
		remove_write_avail.write_unlock();
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
		if(new_read_avail.write_lock()) {
			new_read_avail.input().clear();
		}
		if(new_write_avail.write_lock()) {
			new_write_avail.input().clear();
		}
		if(remove_read_avail.write_lock()) {
			remove_read_avail.input().clear();
		}
		for(int i = 0; i < num_events; ++i) {
			epoll_event& event = new_events[i];
			stream_state_t& state = state_vector[event.data.u64];
			if(state.sock < 0) {
				continue;
			}
			if(state.sock == notify_pipe[0]) {
				char buf[4096];
				while(::read(notify_pipe[0], buf, sizeof(buf)) == sizeof(buf));
				continue;
			}
			if(event.events & EPOLLIN && !state.is_paused) {
				new_read_avail.input().push_back(std::make_pair(state.id, state.sock));
			}
			if(event.events & EPOLLOUT) {
				new_write_avail.input().push_back(std::make_pair(state.id, state.sock));
				event.events = EPOLLIN | EPOLLONESHOT;
				::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, state.sock, &event);
			}
		}
		new_read_avail.write_unlock();
		new_write_avail.write_unlock();
		remove_read_avail.write_unlock();
	}
	read_loop->notify();
	write_loop->notify();
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
	if(write_loop) {
		write_loop->close();
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
	write_loop = std::make_shared<WriteLoop>();
	
	accept_loop->poll_loop = poll_loop;
	accept_loop->server_sock = server_sock;
	accept_loop->endpoint = endpoint;
	
	poll_loop->frontend = this;
	poll_loop->accept_loop = accept_loop;
	poll_loop->read_loop = read_loop;
	poll_loop->write_loop = write_loop;
	
	read_loop->frontend = this;
	read_loop->poll_loop = poll_loop;
	
	write_loop->frontend = this;
	write_loop->poll_loop = poll_loop;
	
	accept_loop->start();
	poll_loop->start();
	read_loop->start();
	write_loop->start();
	
	subscribe(input, 0);
	subscribe(channel);
	
	set_timer_millis(1000, std::bind(&Frontend::update, this));
	
	log(INFO).out << "running on " << endpoint->to_url();
}

void Frontend::handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) {
	if(new_event_list.write_lock()) {
		new_event_list.input().clear();
	}
	new_event_list.input().insert(new_event_list.input().end(), value->array.begin(), value->array.end());
	new_event_list.write_unlock();
	poll_loop->notify();
}

void Frontend::handle(std::shared_ptr<const ::vnx::web::StreamWrite> value) {
	if(new_write_data.write_lock()) {
		new_write_data.input().clear();
	}
	new_write_data.input().push_back(value);
	new_write_data.write_unlock();
	write_loop->notify();
}

void Frontend::update() {
	log(INFO).out << "connections=" << (num_connections - 1) << ", accept=" << num_accept << "/s, timeout="
			<< num_timeout << "/s, close=" << num_close << "/s"
			<< ", read=" << (float(num_bytes_read / 1024) / 1024) << " MB/s, write="
			<< (float(num_bytes_written / 1024) / 1024) << " MB/s";
	num_accept = 0;
	num_bytes_read = 0;
	num_bytes_written = 0;
	num_timeout = 0;
	num_close = 0;
}


} // web
} // vnx
