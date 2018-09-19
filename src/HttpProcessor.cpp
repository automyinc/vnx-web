
#include <vnx/web/HttpProcessor.h>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/BinaryData.hxx>
#include <vnx/web/File.hxx>
#include <vnx/web/Util.h>
#include <vnx/web/SetCookie.hxx>

#include <sstream>


namespace vnx {
namespace web {

HttpProcessor::HttpProcessor(const std::string& _vnx_name)
	:	HttpProcessorBase(_vnx_name)
{
}

void HttpProcessor::init() {
	input_pipe = subscribe(input);
	subscribe(channel, 0);				// we never want to miss a response
}

void HttpProcessor::main() {
	
	server_start_time_ms = vnx::get_time_millis();
	
	client = std::make_shared<DatabaseClient>("Database");
	
	if(cache_control.count("text/html") == 0) {
		cache_control["text/html"] = "no-cache, must-revalidate";
	}
	
	set_timer_millis(1000, std::bind(&HttpProcessor::print_stats, this));
	set_timer_millis(60000, std::bind(&HttpProcessor::write_stats, this));
	
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

void HttpProcessor::process(state_t& state, std::shared_ptr<const HttpRequest> request) {
	
	std::shared_ptr<Request> forward;
	if(request->method == "GET" || request->method == "HEAD") {
		forward = Request::create();
		forward->path = request->path;
		forward->type = request_type_e::READ;
	} else if(request->method == "POST") {
		forward = Request::create();
		forward->path = request->path;
		forward->type = request_type_e::GENERIC;
	}
	
	std::map<std::string, std::string> header_fields;
	for(const auto& field : request->header) {
		if(field.first == "Cookie") {
			if(forward) {
				const std::vector<std::string> cookies = vnx::string_split(field.second, ';');
				for(const auto& cookie : cookies) {
					const std::vector<std::string> pair = vnx::string_split(cookie, '=');
					if(pair.size() == 2) {
						std::string name;
						if(!pair[0].empty() && pair[0][0] == ' ') {
							name = pair[0].substr(1);
						} else {
							name = pair[0];
						}
						forward->session[name] = pair[1];
					}
				}
			}
		} else {
			header_fields[field.first] = field.second;
		}
	}
	
	std::string domain = default_domain;
	{
		auto iter = header_fields.find("Host");
		if(iter != header_fields.end()) {
			if(domain_map.count(iter->second)) {
				domain = iter->second;
			}
		}
	}
	
	domain_stats_t& stats = domain_stats[domain];
	{
		auto iter = header_fields.find("Referer");
		if(iter != header_fields.end()) {
			std::string url = iter->second;
			for(size_t i = 0; i < url.size(); ++i) {
				if(url[i] == '?') {
					url.resize(i);
					break;
				}
			}
			if(url.size() > 256) {
				url.resize(256);
			}
			auto it = stats.referral_count.find(url);
			if(it != stats.referral_count.end()) {
				it->second++;
			} else if(stats.referral_count.size() < max_num_pages) {
				stats.referral_count[url]++;
			}
		}
	}
	
	if(forward) {
		auto iter = domain_map.find(domain);
		if(iter != domain_map.end()) {
			if(forward->path == "/favicon.ico") {
				state.response_map[request->id] = Response::create(request,
						ErrorCode::create_with_message(ErrorCode::MOVED_TEMPORARILY, "/file/favicon.ico"));
			} else if(forward->path.is_root()) {
				state.response_map[request->id] = Response::create(request,
						ErrorCode::create_with_message(ErrorCode::MOVED_TEMPORARILY, index_path.to_string()));
			} else {
				forward->id = request->id;
				forward->stream = request->stream;
				forward->parameter = request->parameter;
				forward->source.push_back(channel);
				forward->time_stamp_ms = request->time_stamp_ms;
				forward->timeout_ms = timeout_ms;
				publish(forward, iter->second, BLOCKING);
				
				request_entry_t& entry = pending_requests[request->id];
				entry.stream = state.stream;
				entry.domain = domain;
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
	out->header.emplace_back("Server", "vnx::web");
	if(keepalive) {
		out->header.emplace_back("Connection", "keep-alive");
	} else {
		out->header.emplace_back("Connection", "close");
	}
	out->do_close = !keepalive || do_close;
	
	auto result = response->result;
	domain_stats_t& stats = domain_stats[domain];
	{
		auto error = std::dynamic_pointer_cast<const ErrorCode>(result);
		if(error) {
			out->status = error->code;
			out->result = get_error_content(error->code);
			if(error->code == ErrorCode::MOVED_PERMANENTLY) {
				out->header.emplace_back("Location", error->message);
			}
			if(error->code == ErrorCode::MOVED_TEMPORARILY) {
				out->header.emplace_back("Location", error->message);
			}
			{
				auto cookie = std::dynamic_pointer_cast<const SetCookie>(error);
				if(cookie) {
					out->header.emplace_back("Set-Cookie", cookie->name + "=" + cookie->value);
				}
			}
			stats.error_count[error->code]++;
		} else {
			out->status = 200;
			out->result = result;
			auto content = std::dynamic_pointer_cast<const Content>(result);
			if(response->is_dynamic) {
				out->header.emplace_back("Cache-Control", "no-cache, no-store, must-revalidate");
			} else {
				if(content) {
					const int64_t time_stamp_s = content->time_stamp_ms / 1000;
					out->header.emplace_back("Last-Modified", get_date_string(time_stamp_s));
					out->header.emplace_back("ETag", "\"" + request->path.get_hash().to_string()
							+ ":" + std::to_string(time_stamp_s) + "\"");
					{
						auto iter = cache_control.find(content->mime_type);
						if(iter != cache_control.end()) {
							out->header.emplace_back("Cache-Control", iter->second);
						} else {
							if(!default_cache_control.empty()) {
								out->header.emplace_back("Cache-Control", default_cache_control);
							}
						}
					}
				} else {
					out->header.emplace_back("Cache-Control", "no-cache");
				}
			}
			{
				std::string path = request->path.to_string();
				if(path.size() > 256) {
					path.resize(256);
				}
				auto iter = stats.page_count.find(path);
				if(iter != stats.page_count.end()) {
					iter->second++;
				} else if(stats.page_count.size() < max_num_pages) {
					stats.page_count[path]++;
				}
			}
		}
	}
	if(request->method == "HEAD") {
		out->is_head_response = true;
	}
	publish(out, output, BLOCKING);
}

void HttpProcessor::print_stats() {
	log(INFO).out << "requests=" << request_counter << "/s, pending=" << pending_requests.size()
			<< ", reject=" << reject_counter << "/s";
	request_counter = 0;
	reject_counter = 0;
}

void HttpProcessor::write_stats() {
	const int64_t now = vnx::get_time_millis();
	try {
		for(const auto& entry : domain_stats) {
			{
				std::map<Hash128, Object> rows;
				for(const auto& count : entry.second.error_count) {
					Object& row = rows[Hash128::rand()];
					row["time"] = now;
					row["domain"] = entry.first;
					row["error"] = count.first;
					row["count"] = count.second;
				}
				client->insert_many_async("error_count", rows);
			}
			{
				std::map<Hash128, Object> rows;
				for(const auto& count : entry.second.page_count) {
					Object& row = rows[Hash128::rand()];
					row["time"] = now;
					row["domain"] = entry.first;
					row["path"] = count.first;
					row["count"] = count.second;
				}
				client->insert_many_async("page_count", rows);
			}
			{
				std::map<Hash128, Object> rows;
				for(const auto& count : entry.second.referral_count) {
					Object& row = rows[Hash128::rand()];
					row["time"] = now;
					row["domain"] = entry.first;
					row["url"] = count.first;
					row["count"] = count.second;
				}
				client->insert_many_async("referral_count", rows);
			}
		}
	} catch(...) {
		log(WARN).out << "Failed to commit statistics to database!";
	}
	domain_stats.clear();
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

std::string HttpProcessor::get_date_string(int64_t time_stamp_s) {
	auto iter = date_string_map.find(time_stamp_s);
	if(iter != date_string_map.end()) {
		return iter->second;
	}
	const std::string str = to_date_string(time_stamp_s);
	date_string_map[time_stamp_s] = str;
	return str;
}


} // web
} // vnx
