
#ifndef INCLUDE_VNX_WEB_FRONTEND_H
#define INCLUDE_VNX_WEB_FRONTEND_H

#include <vnx/web/FrontendBase.hxx>
#include <vnx/web/DoubleBuffer.h>
#include <vnx/Endpoint.hxx>

#include <unordered_map>


namespace vnx {
namespace web {

class Frontend : public FrontendBase {
public:
	Frontend(const std::string& _vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamWrite> value) override;
	
private:
	void setup();
	
	void accept_loop();
	
	void poll_loop();
	
	void read_loop();
	
	void write_loop();
	
	void notify_poll_loop();
	
private:
	std::shared_ptr<const Endpoint> endpoint;
	Timer* setup_timer = 0;
	int server_sock = -1;
	int server_pipe[2] = {-1, -1};
	
	DoubleBuffer<std::vector<int>> new_sockets;
	DoubleBuffer<std::vector<int>> read_avail;
	DoubleBuffer<std::vector<int>> write_avail;
	DoubleBuffer<std::vector<stream_event_t>> new_events;
	DoubleBuffer<std::vector<std::shared_ptr<const StreamWrite>>> new_write_data;
	
	std::thread accept_thread;
	std::thread poll_thread;
	std::thread read_thread;
	std::thread write_thread;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_FRONTEND_H
