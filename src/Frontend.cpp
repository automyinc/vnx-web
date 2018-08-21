
#include <vnx/web/Frontend.h>

#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>


namespace vnx {
namespace web {

Frontend::Frontend(const std::string& _vnx_name)
	:	FrontendBase(_vnx_name)
{
}

void Frontend::init() {
	
}

void Frontend::main() {
	
	endpoint = Endpoint::from_url(address);
	
	setup_timer = set_timer_millis(1000, std::bind(&Frontend::setup, this));
	
	setup();
	
	Super::main();
	
	if(server_sock >= 0) {
		::shutdown(server_sock, SHUT_RDWR);
	}
	if(accept_thread.joinable()) {
		accept_thread.join();
	}
	if(poll_thread.joinable()) {
		poll_thread.join();
	}
	if(read_thread.joinable()) {
		read_thread.join();
	}
	if(write_thread.joinable()) {
		write_thread.join();
	}
}

void Frontend::handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) {
	if(new_events.write_lock()) {
		new_events.input().clear();
	}
	new_events.input().insert(new_events.input().end(), value->array.begin(), value->array.end());
	new_events.write_unlock();
	notify_poll_loop();
}

void Frontend::handle(std::shared_ptr<const ::vnx::web::StreamWrite> value) {
	if(new_write_data.write_lock()) {
		new_write_data.input().clear();
	}
	new_write_data.input().push_back(value);
	new_write_data.write_unlock();
}

void set_non_block(int fd) {
	if(::fcntl(fd, F_SETFL, ::fcntl(fd, F_GETFL) | O_NONBLOCK) != 0) {
		throw std::runtime_error("fcntl(F_SETFL, O_NONBLOCK) failed with: " + std::to_string(errno));
	}
}

void Frontend::setup() {
	try {
		if(server_pipe[0] < 0 || server_pipe[1] < 0) {
			if(::pipe(server_pipe) != 0) {
				throw std::runtime_error("pipe() failed with: " + std::to_string(errno));
			}
		}
		set_non_block(server_pipe[0]);
		set_non_block(server_pipe[1]);
		if(server_sock < 0) {
			server_sock = endpoint->open();
		}
		endpoint->bind(server_sock);
		endpoint->listen(server_sock);
	} catch(std::exception& ex) {
		log(WARN).out << ex.what();
		return;
	}
	accept_thread = std::thread(&Frontend::accept_loop, this);
	poll_thread = std::thread(&Frontend::poll_loop, this);
	read_thread = std::thread(&Frontend::read_loop, this);
	write_thread = std::thread(&Frontend::write_loop, this);
	setup_timer->stop();
	log(INFO).out << "running on " << endpoint->to_url();
}

void Frontend::accept_loop() {
	try {
		while(true) {
			const int socket = endpoint->accept(server_sock);
			set_non_block(socket);
			if(new_sockets.write_lock()) {
				new_sockets.input().clear();
			}
			new_sockets.input().push_back(socket);
			new_sockets.write_unlock();
			notify_poll_loop();
		}
	} catch(...) {
		::close(server_sock);
		server_sock = -1;
	}
	if(new_sockets.write_lock()) {
		new_sockets.input().clear();
	}
	new_sockets.input().push_back(-1);
	new_sockets.write_unlock();
	exit();
}

::pollfd create_pollfd(int sock, short events) {
	::pollfd res = {};
	res.fd = sock;
	res.events = events;
	return res;
}

void Frontend::poll_loop() {
	std::vector<::pollfd> streams;
	streams.push_back(create_pollfd(server_pipe[0], POLLIN));
	try {
		while(true) {
			
		}
	} catch(...) {
		
	}
	exit();
}

void Frontend::read_loop() {
	try {
		while(true) {
			
		}
	} catch(...) {
		
	}
	exit();
}

void Frontend::write_loop() {
	try {
		while(true) {
			
		}
	} catch(...) {
		
	}
	exit();
}

void Frontend::notify_poll_loop() {
	const char data = 0;
	::write(server_pipe[1], &data, 1);
}


} // web
} // vnx
