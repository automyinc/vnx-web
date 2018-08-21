
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
	
	set_timer_millis(update_interval_ms, std::bind(&HttpProcessor::update, this));
	
	Super::main();
}

void HttpProcessor::handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) {
	for(const stream_event_t& event : value->array) {
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
	publish(value, output);
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
		state_t& state = state_map[iter->second];
		state.response_map[response->id] = response;
		process(state);
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

void HttpProcessor::process(state_t& state) {
	while(!state.request_queue.empty()) {
		std::shared_ptr<const HttpRequest> request = state.request_queue.front();
		auto iter = state.response_map.find(request->id);
		if(iter != state.response_map.end()) {
			process(state, request, iter->second);
			state.response_map.erase(request->id);
			state.request_queue.pop();
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
	
	if(request->sequence <= state.sequence) {
		log(WARN).out << "Duplicate request: sequence=" << request->sequence << ", id=" << request->id;
		return;
	}
	if(request->sequence != state.sequence + 1) {
		log(WARN).out << "Out of order request: sequence=" << request->sequence << ", id=" << request->id;
	}
	state.sequence = request->sequence;
	
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
	
	total_queue_size++;
	state.request_queue.push(request);
	process(state);
	
	if(state.request_queue.size() > max_queue_size) {
		pause_stream(state, request->channel);
	}
}

void HttpProcessor::process(state_t& state, std::shared_ptr<const HttpRequest> request, std::shared_ptr<const Response> response) {
	
	std::shared_ptr<HttpResponse> out = HttpResponse::create();
	out->id = request->id;
	out->stream = state.stream;
	out->sequence = request->sequence;
	{
		auto error = std::dynamic_pointer_cast<const ErrorCode>(response->content);
		if(error) {
			switch(error->code) {
				case ErrorCode::BAD_REQUEST: out->status = 400; break;
				case ErrorCode::NOT_FOUND: out->status = 404; break;
				case ErrorCode::TIMEOUT: out->status = 429; break;
				case ErrorCode::INTERNAL_ERROR: out->status = 500; break;
				case ErrorCode::OVERLOAD: out->status = 503; break;
				default: out->status = 500;
			}
		} else {
			out->content = response->content;
			out->status = 200;
		}
	}
	
	publish(out, output);
	
	if(state.request_queue.size() <= max_queue_size / 2) {
		resume_stream(state, request->channel);
	}
}

void HttpProcessor::update() {
	size_t num_paused = 0;
	for(const auto& entry : pause_map) {
		auto event = StreamEventArray::create();
		for(const auto& stream : entry.second) {
			event->array.push_back(stream_event_t::create_with_value(stream, stream_event_t::EVENT_PAUSE, 500));
		}
		publish(event, entry.first);
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




} // web
} // vnx
