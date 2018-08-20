
#include <vnx/web/HttpProcessor.h>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/Parameter.hxx>
#include <vnx/web/BinaryData.hxx>


namespace vnx {
namespace web {

void HttpProcessor::init() {
	input_pipe = subscribe(input);
	subscribe(channel, 0);
}

void HttpProcessor::main() {
	Super::main();
}

void HttpProcessor::handle(std::shared_ptr<const ::vnx::web::StreamEvent> event) {
	switch(event->event) {
		case StreamEvent::EVENT_EOF: {
			state_t& state = state_map[event->stream];
			while(!state.request_queue.empty()) {
				pending_requests.erase(state.request_queue.front()->id);
				state.request_queue.pop();
			}
			state_map.erase(event->stream);
			break;
		}
	}
}

void HttpProcessor::handle(std::shared_ptr<const ::vnx::web::HttpRequest> request) {
	state_t& state = state_map[request->stream];
	state.stream = request->stream;
	process(state, request);
	
	if(input_pipe) {
		const size_t backlog = pending_requests.size();
		if(backlog >= max_backlog) {
			if(!input_pipe->get_is_paused()) {
				log(WARN).out << "Blocked input: backlog = " << backlog << ", size = " << state.response_map.size();
			}
			input_pipe->pause();
		}
	}
}

void HttpProcessor::handle(std::shared_ptr<const ::vnx::web::Response> response) {
	auto iter = pending_requests.find(response->id);
	if(iter != pending_requests.end()) {
		state_t& state = state_map[iter->second];
		state.response_map[response->id] = response;
		process(state);
		pending_requests.erase(response->id);
	}
	
	if(input_pipe) {
		const size_t backlog = pending_requests.size();
		if(backlog <= max_backlog / 2) {
			if(input_pipe->get_is_paused()) {
				log(WARN).out << "Resumed input: backlog = " << backlog;
			}
			input_pipe->resume();
		}
	}
}

void HttpProcessor::process(state_t& state) {
	while(!state.request_queue.empty()) {
		std::shared_ptr<const HttpRequest> request = state.request_queue.front();
		auto iter = state.response_map.find(request->id);
		if(iter != state.response_map.end()) {
			process(state, request, iter->second);
			state.response_map.erase(request->id);
			state.request_queue.pop();
		} else {
			break;
		}
	}
}

std::shared_ptr<Parameter> parse_parameter(std::shared_ptr<Parameter> parameter, const char* str, size_t len) {
	int state = 0;
	std::string key;
	std::string value;
	for(size_t i = 0; i < len; ++i) {
		// TODO
	}
	return parameter;
}

void HttpProcessor::process(state_t& state, std::shared_ptr<const HttpRequest> request) {
	
	std::map<std::string, std::string> header_fields;
	for(const auto& field : request->header) {
		header_fields[field.first] = field.second;
	}
	
	std::shared_ptr<Request> forward;
	std::shared_ptr<Parameter> parameter;
	if(request->method == "GET") {
		forward = Request::create();
		forward->path = request->path;
	} else if(request->method == "POST") {
		forward = Request::create();
		forward->path = request->path;
		const std::string payload = request->payload.as_string();
		parameter = parse_parameter(parameter, payload.c_str(), payload.size());
	}
	if(request->method == "GET" || request->method == "POST") {
		if(!forward->path.empty()) {
			std::string& str = forward->path.back();
			for(size_t i = 0; i < str.size(); ++i) {
				if(str[i] == '?') {
					if(i + 1 < str.size()) {
						parameter = parse_parameter(parameter, &str[i + 1], str.size() - i - 1);
					}
					str.resize(i);
					break;
				}
			}
		}
		forward->parameter = parameter;
	}
	
	if(forward) {
		std::string target_domain = default_domain;
		{
			auto iter = header_fields.find("Host");
			if(iter != header_fields.end()) {
				target_domain = iter->second;
			}
		}
		auto iter = domain_map.find(target_domain);
		if(iter != domain_map.end()) {
			forward->id = request->id;
			forward->channel = channel;
			forward->time_stamp_ms = request->time_stamp_ms;
			forward->timeout_ms = timeout_ms;
			publish(forward, iter->second, BLOCKING);
			pending_requests[request->id] = state.stream;
		} else {
			state.response_map[request->id] = Response::create(request, ErrorCode::create(ErrorCode::NOT_FOUND));
		}
	} else {
		state.response_map[request->id] = Response::create(request, ErrorCode::create(ErrorCode::BAD_REQUEST));
	}
	
	state.request_queue.push(request);
	process(state);
	
	if(state.request_queue.size() > max_queue_size) {
		publish(StreamEvent::create_with_value(request->stream, StreamEvent::EVENT_PAUSE, 50), request->channel);
	}
}

void HttpProcessor::process(state_t& state, std::shared_ptr<const HttpRequest> request, std::shared_ptr<const Response> response) {
	
	std::shared_ptr<HttpResponse> out = HttpResponse::create();
	out->id = request->id;
	out->stream = state.stream;
	out->sequence = request->sequence;
	out->content = response->content;
	
	publish(out, output);
}


} // web
} // vnx
