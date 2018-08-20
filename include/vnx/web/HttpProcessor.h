
#ifndef INCLUDE_VNX_WEB_HTTPPROCESSOR_H
#define INCLUDE_VNX_WEB_HTTPPROCESSOR_H

#include <vnx/web/HttpProcessorBase.hxx>
#include <vnx/web/HttpResponse.hxx>

#include <queue>
#include <unordered_map>


namespace vnx {
namespace web {

class HttpProcessor : public HttpProcessorBase {
public:
	HttpProcessor(const std::string& _vnx_name)
		:	HttpProcessorBase(_vnx_name)
	{
	}

protected:
	struct state_t {
		Hash128 stream;
		std::queue<std::shared_ptr<const HttpRequest>> request_queue;
		std::map<Hash128, std::shared_ptr<const Response>> response_map;
	};
	
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamEvent> event) override;
	
	void handle(std::shared_ptr<const ::vnx::web::HttpRequest> request) override;
	
	void handle(std::shared_ptr<const ::vnx::web::Response> response) override;
	
private:
	void process(state_t& state);
	
	void process(state_t& state, std::shared_ptr<const HttpRequest> request);
	
	void process(state_t& state, std::shared_ptr<const HttpRequest> request, std::shared_ptr<const Response> response);
	
private:
	std::shared_ptr<Pipe> input_pipe;
	std::unordered_map<Hash128, state_t> state_map;
	std::unordered_map<Hash128, Hash128> pending_requests;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_HTTPPROCESSOR_H
