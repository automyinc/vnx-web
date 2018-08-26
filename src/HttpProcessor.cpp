
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
	subscribe(channel, 0);				// we never want to miss a response
}

void HttpProcessor::main() {
	
	server_start_time_ms = vnx::get_time_millis();
	
	set_timer_millis(1000, std::bind(&HttpProcessor::print_stats, this));
	
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
	if(state.is_closed) {
		reject_counter++;
		return;
	}
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
			state.request_queue.pop();
			process(state, domain, request, iter->second, state.is_closed && state.request_queue.empty());
			state.response_map.erase(request->id);
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
			if(!forward->path.is_root()) {
				
				forward->id = request->id;
				forward->stream = request->stream;
				forward->source.push_back(channel);
				forward->time_stamp_ms = request->time_stamp_ms;
				forward->timeout_ms = timeout_ms;
				publish(forward, iter->second, BLOCKING);
				
				request_entry_t& entry = pending_requests[request->id];
				entry.stream = state.stream;
				entry.domain = domain;
			} else {
				state.response_map[request->id] = Response::create(request,
						ErrorCode::create_with_message(ErrorCode::MOVED_PERMANENTLY, index_path.to_string()));
			}
		} else {
			state.response_map[request->id] = Response::create(request, ErrorCode::create(ErrorCode::NOT_FOUND));
		}
	} else {
		state.response_map[request->id] = Response::create(request, ErrorCode::create(ErrorCode::BAD_REQUEST));
	}
	
	state.request_queue.push(request);
	process(state, domain);
	
	if(state.request_queue.size() > max_queue_size) {
		state.is_closed = true;
	}
}

void HttpProcessor::process(	state_t& state, const std::string& domain,
								std::shared_ptr<const HttpRequest> request,
								std::shared_ptr<const Response> response,
								bool do_close)
{
	std::shared_ptr<HttpResponse> out = HttpResponse::create();
	out->id = request->id;
	out->stream = state.stream;
	out->sequence = request->sequence;
	out->is_dynamic = response->is_dynamic;
	out->time_to_live_ms = response->time_to_live_ms;
	out->header.emplace_back("Host", domain);
	out->header.emplace_back("Server", "vnx::web::server");
	if(keepalive) {
		out->header.emplace_back("Connection", "keep-alive");
	} else {
		out->header.emplace_back("Connection", "close");
	}
	out->do_close = !keepalive || do_close;
	
	HttpDomainStats& stats = domain_stats[domain];
	{
		auto error = std::dynamic_pointer_cast<const ErrorCode>(response->content);
		if(error) {
			out->status = error->code;
			out->content = get_error_content(error->code);
			if(error->code == ErrorCode::MOVED_PERMANENTLY) {
				out->header.emplace_back("Location", error->message);
			}
			stats.error_counts[error->code]++;
		} else {
			out->status = 200;
			out->content = response->content;
			stats.page_hits[request->path.to_string()]++;
		}
	}
	publish(out, output, BLOCKING);
}

void HttpProcessor::print_stats() {
	for(const auto& entry : domain_stats) {
		auto request = Request::create();
		request->id = Hash128::rand();
		request->type = request_type_e::WRITE;
		request->path = "/file/vnx.web/http_domain_stats_" + std::to_string(server_start_time_ms) + ".dat";
		auto data = BinaryData::create();
		data->write_value(entry.second);
		request->parameter = data;
		request->time_stamp_ms = vnx::get_time_millis();
		publish(request, domain_map[entry.first]);
	}
	
	log(INFO).out << "requests=" << request_counter << "/s, pending=" << pending_requests.size()
			<< ", reject=" << reject_counter << "/s";
	request_counter = 0;
	reject_counter = 0;
}

std::shared_ptr<File> create_error_page(int code) {
	std::shared_ptr<File> file = File::create();
	file->mime_type = "text/html";
	file->time_stamp_ms = vnx::get_time_millis();
	
	std::ostringstream tmp;
	tmp << "<html>\n<body>\n<h2>Error: " << std::to_string(code)
			<< " " << ErrorCode::get_error_string(code) << "</h2>\n</body>\n</html>\n";
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
