
#include <vnx/web/Frontend.h>
#include <vnx/web/StreamRead.hxx>

#include <mutex>
#include <condition_variable>

#include <queue>
#include <unordered_map>

#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>


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
	std::mutex mutex;
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
		try {
			while(do_run) {
				const int sock = endpoint->accept(server_sock);
				add_socket(sock);
//				std::cout << "accept: " << sock << std::endl;
			}
		} catch(...) {
			
		}
		
		add_socket(-1);
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
	
	DoubleBuffer<std::vector<std::pair<Hash128, int>>> new_read_avail;
	DoubleBuffer<std::vector<std::pair<Hash128, int>>> new_write_avail;
	DoubleBuffer<std::vector<Hash128>> remove_read_avail;
	DoubleBuffer<std::vector<Hash128>> remove_write_avail;
	
	void notify() override {
		const char data = 0;
		const ssize_t res = ::write(notify_pipe[1], &data, 1);
	}

protected:
	struct stream_state_t {
		Hash128 id;
		int64_t resume_time = -1;
		bool is_paused = false;
	};
	
	int notify_pipe[2] = {-1, -1};
	std::unordered_map<Hash128, uint32_t> stream_map;
	std::vector<stream_state_t> state_vector;
	std::vector<::pollfd> poll_vector;
	std::vector<uint32_t> free_list;
	
	::pollfd create_pollfd(int sock, short events) {
		::pollfd res = {};
		res.fd = sock;
		res.events = events;
		return res;
	}
	
	Hash128 add_stream(int sock, short events) {
		uint32_t index;
		if(free_list.empty()) {
			index = uint32_t(state_vector.size());
			state_vector.resize(index + 1);
			poll_vector.resize(index + 1);
		} else {
			index = free_list.back();
			free_list.pop_back();
		}
		stream_state_t& state = state_vector[index];
		state = stream_state_t();
		state.id = Hash128::rand();
		stream_map[state.id] = index;
		poll_vector[index] = create_pollfd(sock, events);
		frontend->num_connections++;
		frontend->num_accept++;
		return state.id;
	}
	
	void remove_stream(size_t index) {
		stream_state_t& state = state_vector[index];
		stream_map.erase(state.id);
		poll_vector[index].fd = -1;
		poll_vector[index].events = 0;
		poll_vector[index].revents = 0;
		free_list.push_back(uint32_t(index));
		frontend->num_connections--;
		frontend->num_close++;
	}
	
	void init() override {
		if(::pipe(notify_pipe) != 0) {
			throw std::runtime_error("pipe() failed with: " + std::to_string(errno));
		}
		set_non_block(notify_pipe[0]);
		set_non_block(notify_pipe[1]);
	}
	
	void main() override {
		
		add_stream(notify_pipe[0], POLLIN);
		
		while(do_run) {
			loop();
		}
		
		auto events = StreamEventArray::create();
		for(size_t i = 0; i < state_vector.size(); ++i) {
			if(poll_vector[i].fd >= 0) {
				events->array.push_back(stream_event_t::create(state_vector[i].id, stream_event_t::EVENT_EOF));
				::close(poll_vector[i].fd);
			}
		}
		publish(events, frontend->output);
		
		accept_loop = 0;
		read_loop = 0;
		write_loop = 0;
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
//					std::cout << "read_set[...] = " << entry.second << std::endl;
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
//					std::cout << "remove_list.push_back = " << entry.second << " (num_read=" << num_read << ")" << std::endl;
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
		int sock = -1;
	};
	
	int64_t last_update_time = 0;
	size_t write_block_size = 0;
	std::unordered_map<Hash128, stream_state_t> stream_map;
	std::unordered_map<Hash128, write_state_t> write_set;
	
	void main() override {
		
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
						const ssize_t num_written = ::send(state.sock, buffer.data(state.offset), size_t(num_left), MSG_NOSIGNAL);
						if(num_written > 0) {
							frontend->num_bytes_written += num_written;
						}
//						std::cout << "sock=" << state.sock << ", num_left=" << num_left << ", num_written=" << num_written << ", index=" << state.index << ", offset=" << state.offset << std::endl;
						if(num_written == num_left) {
							state.index++;
							state.offset = 0;
							total_written += num_written;
							if(total_written >= write_block_size) {
								break;
							}
						} else {
							state.sock = -1;
							if(num_written > 0) {
								state.offset += num_written;
							}
							set_poll_out.input().push_back(entry.first);
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
						if(write_set.count(entry.first) == 0) {
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
				const Hash128 id = add_stream(sock, POLLIN);
				new_write_avail.input().push_back(std::make_pair(id, sock));
				stream_events->array.push_back(stream_event_t::create(id, stream_event_t::EVENT_NEW));
//				std::cout << "new socket: " << sock << std::endl;
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
					auto& events = poll_vector[iter->second].events;
					if(events == 0) {
						events = POLLIN;
//						std::cout << "set POLLIN on " << poll_vector[iter->second].fd << std::endl;
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
					poll_vector[iter->second].events = POLLOUT;
					remove_read_avail.input().push_back(state_vector[iter->second].id);
					std::cout << "set POLLOUT on " << poll_vector[iter->second].fd << std::endl;
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
					poll_vector[iter->second].revents = POLLHUP;
//					std::cout << "set POLLHUP on " << poll_vector[iter->second].fd << std::endl;
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
					poll_vector[iter->second].revents = POLLHUP;
//					std::cout << "set POLLHUP on " << poll_vector[iter->second].fd << std::endl;
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
//				std::cout << "new event: " << event << std::endl;
				stream_state_t& state = state_vector[iter->second];
				::pollfd& entry = poll_vector[iter->second];
				switch(event.event) {
					case stream_event_t::EVENT_PAUSE:
						state.resume_time = now + event.value;
						state.is_paused = true;
						break;
					case stream_event_t::EVENT_RESUME:
						state.resume_time = now;
						break;
					case stream_event_t::EVENT_CLOSE:
						if(entry.fd >= 0) {
							entry.revents |= POLLHUP;
						}
						break;
				}
			}
			frontend->new_event_list.read_unlock();
		}
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
		if(remove_write_avail.write_lock()) {
			remove_write_avail.input().clear();
		}
		for(size_t i = 0; i < state_vector.size(); ++i) {
			::pollfd& entry = poll_vector[i];
			if(entry.fd < 0) {
				continue;
			}
			stream_state_t& state = state_vector[i];
			if(state.is_paused) {
				if(now < state.resume_time) {
					if(entry.events == 0) {
						remove_read_avail.input().push_back(state.id);
					}
					if(entry.events == POLLIN) {
						entry.events = 0;
					}
				} else {
					if(entry.events == 0) {
						entry.events = POLLIN;
					}
					state.is_paused = false;
				}
			}
			if(entry.events == POLLIN && entry.revents & POLLIN) {
//				std::cout << "got POLLIN on " << entry.fd << std::endl;
				new_read_avail.input().push_back(std::make_pair(state.id, entry.fd));
				entry.events = 0;
			}
			if(entry.events == POLLOUT && entry.revents & POLLOUT) {
//				std::cout << "got POLLOUT on " << entry.fd << std::endl;
				new_write_avail.input().push_back(std::make_pair(state.id, entry.fd));
				entry.events = POLLIN;
			}
			if(entry.revents & (POLLHUP | POLLERR | POLLNVAL)) {
//				std::cout << "got POLLHUP | POLLERR | POLLNVAL on " << entry.fd << std::endl;
				remove_write_avail.input().push_back(state.id);
				stream_events->array.push_back(stream_event_t::create(state.id, stream_event_t::EVENT_EOF));
				const int res = ::close(entry.fd);
				remove_stream(i);
			}
			entry.revents = 0;
		}
		new_read_avail.write_unlock();
		new_write_avail.write_unlock();
		remove_read_avail.write_unlock();
		remove_write_avail.write_unlock();
	}
	read_loop->notify();
	write_loop->notify();
	
	if(!stream_events->array.empty()) {
		publish(stream_events, frontend->output);
	}
	
	const int res = ::poll(&poll_vector[0], poll_vector.size(), 100);
	if(res < 0) {
		do_run = false;
	}
//	std::cout << "poll(size=" << poll_vector.size() << ") = " << res << std::endl;
	
	{
		::pollfd& entry = poll_vector[0];
		if(entry.revents & POLLIN) {
			char buf[4096];
			while(::read(entry.fd, buf, sizeof(buf)) == sizeof(buf));
			entry.revents = 0;
		}
	}
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
