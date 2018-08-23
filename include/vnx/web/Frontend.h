
#ifndef INCLUDE_VNX_WEB_FRONTEND_H
#define INCLUDE_VNX_WEB_FRONTEND_H

#include <vnx/web/FrontendBase.hxx>
#include <vnx/web/DoubleBuffer.h>
#include <vnx/Endpoint.hxx>


namespace vnx {
namespace web {

class Frontend : public FrontendBase {
public:
	Frontend(const std::string& _vnx_name);
	
protected:
	class AcceptLoop;
	class PollLoop;
	class ReadLoop;
	class WriteLoop;
	
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamWrite> value) override;
	
private:
	void setup();
	
private:
	int server_sock = -1;
	Timer* setup_timer = 0;
	std::shared_ptr<Endpoint> endpoint;
	
	DoubleBuffer<std::vector<stream_event_t>> new_event_list;							// main() -> poll_loop
	DoubleBuffer<std::vector<std::shared_ptr<const StreamWrite>>> new_write_data;		// main() -> write_loop
	
	std::shared_ptr<AcceptLoop> accept_loop;
	std::shared_ptr<PollLoop> poll_loop;
	std::shared_ptr<ReadLoop> read_loop;
	std::shared_ptr<WriteLoop> write_loop;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_FRONTEND_H
