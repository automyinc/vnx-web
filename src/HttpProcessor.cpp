
#include <vnx/web/HttpProcessor.h>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/Parameter.hxx>
#include <vnx/web/BinaryData.hxx>
#include <vnx/web/File.hxx>

#include <sstream>


namespace vnx {
namespace web {

void HttpProcessor::init() {
	input_pipe = subscribe(input);
	subscribe(channel, 0);
}

void HttpProcessor::main() {
	
	set_timer_millis(update_interval_ms, std::bind(&HttpProcessor::update, this));
	
	Super::main();
}

void HttpProcessor::handle(std::shared_ptr<const ::vnx::web::StreamEventArray> events) {
	for(const stream_event_t& event : events->array) {
		switch(event.event) {
			case stream_event_t::EVENT_EOF: {
				state_t& state = state_map[event.stream];
				while(!state.request_queue.empty()) {
					pending_requests.erase(state.request_queue.front()->id);
					state.request_queue.pop();
				}
				state_map.erase(event.stream);
				break;
			}
		}
	}
	publish(events, output, BLOCKING);
}

void HttpProcessor::handle(std::shared_ptr<const ::vnx::web::HttpRequest> request) {
	request_counter++;
	
	state_t& state = state_map[request->stream];
	state.stream = request->stream;
	process(state, request);
	
	if(input_pipe) {
		const size_t backlog = pending_requests.size();
		if(backlog >= max_pending) {
			if(!input_pipe->get_is_paused()) {
				log(WARN).out << "Blocked input: pending=" << backlog;
			}
			input_pipe->pause();
		}
	}
}

void HttpProcessor::handle(std::shared_ptr<const ::vnx::web::Response> response) {
	auto iter = pending_requests.find(response->id);
	if(iter != pending_requests.end()) {
		state_t& state = state_map[iter->second.stream];
		state.response_map[response->id] = response;
		process(state, iter->second.domain);
		
		if(state.request_queue.empty()) {
			state_map.erase(state.stream);
		}
		pending_requests.erase(response->id);
	} else {
		log(WARN).out << "Unknown response: id=" << response->id;
	}
	
	if(input_pipe) {
		const size_t backlog = pending_requests.size();
		if(backlog <= max_pending / 2) {
			if(input_pipe->get_is_paused()) {
				log(WARN).out << "Resumed input: pending=" << backlog;
			}
			input_pipe->resume();
		}
	}
}

void HttpProcessor::process(state_t& state, const std::string& domain) {
	while(!state.request_queue.empty()) {
		std::shared_ptr<const HttpRequest> request = state.request_queue.front();
		auto iter = state.response_map.find(request->id);
		if(iter != state.response_map.end()) {
			process(state, domain, request, iter->second);
			state.response_map.erase(request->id);
			state.request_queue.pop();
			if(state.request_queue.size() <= max_queue_size / 2) {
				resume_stream(state, request->channel);
			}
			total_queue_size--;
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
		forward->type = request_type_e::READ;
	} else if(request->method == "POST") {
		forward = Request::create();
		forward->path = request->path;
		forward->type = request_type_e::GENERIC;
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
	
	std::string domain = default_domain;
	if(forward) {
		{
			auto iter = header_fields.find("Host");
			if(iter != header_fields.end()) {
				if(domain_map.count(iter->second)) {
					domain = iter->second;
				}
			}
		}
		auto iter = domain_map.find(domain);
		if(iter != domain_map.end()) {
			forward->id = request->id;
			forward->channel = channel;
			forward->time_stamp_ms = request->time_stamp_ms;
			forward->timeout_ms = timeout_ms;
			publish(forward, iter->second, BLOCKING);
			
			request_entry_t& entry = pending_requests[request->id];
			entry.stream = state.stream;
			entry.domain = domain;
		} else {
			state.response_map[request->id] = Response::create(request, ErrorCode::create(ErrorCode::NOT_FOUND));
		}
	} else {
		state.response_map[request->id] = Response::create(request, ErrorCode::create(ErrorCode::BAD_REQUEST));
	}
	
	total_queue_size++;
	state.request_queue.push(request);
	process(state, domain);
	
	if(state.request_queue.size() > max_queue_size) {
		pause_stream(state, request->channel);
	}
}

void HttpProcessor::process(state_t& state, const std::string& domain, std::shared_ptr<const HttpRequest> request, std::shared_ptr<const Response> response) {
	
	std::shared_ptr<HttpResponse> out = HttpResponse::create();
	out->id = request->id;
	out->stream = state.stream;
	out->sequence = request->sequence;
	out->is_dynamic = response->is_dynamic;
	out->time_to_live_ms = response->time_to_live_ms;
	out->header.push_back(std::make_pair("Host", domain));
	out->header.push_back(std::make_pair("Server", "vnx::web::server"));
	if(keepalive) {
		out->header.push_back(std::make_pair("Connection", "keep-alive"));
	} else {
		out->header.push_back(std::make_pair("Connection", "close"));
	}
	out->do_close = !keepalive;
	{
		auto error = std::dynamic_pointer_cast<const ErrorCode>(response->content);
		if(error) {
			out->status = error->code;
			out->content = get_error_content(error->code);
		} else {
			out->status = 200;
			out->content = response->content;
		}
	}
	publish(out, output, BLOCKING);
}

void HttpProcessor::update() {
	size_t num_paused = 0;
	for(const auto& entry : pause_map) {
		auto events = StreamEventArray::create();
		for(const auto& stream : entry.second) {
			events->array.push_back(stream_event_t::create_with_value(stream, stream_event_t::EVENT_PAUSE, 500));
		}
		if(!events->array.empty()) {
			publish(events, entry.first);
		}
		num_paused += entry.second.size();
	}
	log(INFO).out << "requests=" << ((1000 * request_counter) / update_interval_ms) << "/s, pending="
			<< pending_requests.size() << ", total_queue=" << total_queue_size << ", num_paused=" << num_paused;
	request_counter = 0;
}

void HttpProcessor::pause_stream(state_t& state, const TopicPtr& channel) {
	if(!state.is_paused) {
		pause_map[channel].insert(state.stream);
		auto event = StreamEventArray::create();
		event->array.push_back(stream_event_t::create_with_value(state.stream, stream_event_t::EVENT_PAUSE, 500));
		publish(event, channel);
	}
	state.is_paused = true;
}

void HttpProcessor::resume_stream(state_t& state, const TopicPtr& channel) {
	if(state.is_paused) {
		pause_map[channel].erase(state.stream);
		auto event = StreamEventArray::create();
		event->array.push_back(stream_event_t::create(state.stream, stream_event_t::EVENT_RESUME));
		publish(event, channel);
	}
	state.is_paused = false;
}

std::shared_ptr<File> create_error_page(int code) {
	std::shared_ptr<File> file = File::create();
	file->path = "/error/" + std::to_string(code);
	file->mime_type = "text/html";
	file->time_stamp_ms = vnx::get_time_millis();
	
	std::ostringstream tmp;
	tmp << "<html>\n<body>\n<h1>Error: " << std::to_string(code) << "</h1>\n</body>\n</html>";
	file->data = tmp.str();
	return file;
}

std::shared_ptr<File> HttpProcessor::get_error_content(int code) {
	auto iter = static_error_pages.find(code);
	if(iter != static_error_pages.end()) {
		return iter->second;
	}
	std::shared_ptr<File> content = create_error_page(code);
	static_error_pages[code] = content;
	return content;
}


} // web
} // vnx
