
#ifndef INCLUDE_VNX_WEB_HTTPPROCESSOR_H
#define INCLUDE_VNX_WEB_HTTPPROCESSOR_H

#include <vnx/web/HttpProcessorBase.hxx>
#include <vnx/web/HttpResponse.hxx>

#include <queue>
#include <unordered_set>
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
		std::unordered_map<Hash128, std::shared_ptr<const Response>> response_map;
		bool is_paused = false;
	};
	
	struct request_entry_t {
		Hash128 stream;
		std::string domain;
	};
	
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamEventArray> events) override;
	
	void handle(std::shared_ptr<const ::vnx::web::HttpRequest> request) override;
	
	void handle(std::shared_ptr<const ::vnx::web::Response> response) override;
	
private:
	void process(state_t& state, const std::string& domain);
	
	void process(state_t& state, std::shared_ptr<const HttpRequest> request);
	
	void process(state_t& state, const std::string& domain, std::shared_ptr<const HttpRequest> request, std::shared_ptr<const Response> response);
	
	void update();
	
	void pause_stream(state_t& state, const TopicPtr& channel);
	
	void resume_stream(state_t& state, const TopicPtr& channel);
	
	std::shared_ptr<File> get_error_content(int code);
	
private:
	std::shared_ptr<Pipe> input_pipe;
	std::unordered_map<Hash128, state_t> state_map;
	std::unordered_map<Hash128, request_entry_t> pending_requests;
	std::unordered_map<TopicPtr, std::unordered_set<Hash128>> pause_map;
	
	std::map<int, std::shared_ptr<File>> static_error_pages;
	
	size_t request_counter = 0;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_HTTPPROCESSOR_H
