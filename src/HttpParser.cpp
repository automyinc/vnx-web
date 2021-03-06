
#include <vnx/web/HttpParser.h>
#include <vnx/Object.h>

#include <cstring>


namespace vnx {
namespace web {

HttpParser::HttpParser(const std::string& _vnx_name)
	:	HttpParserBase(_vnx_name)
{
}

HttpParser::state_t::state_t() {
	::http_parser_init(&parser, ::HTTP_REQUEST);
}

int HttpParser::on_message_begin(http_parser* parser) {
	state_t* state = (state_t*)parser->data;
	state->size = 0;
	state->url.clear();
	state->last_was_field = false;
	state->request = HttpRequest::create();
	state->request->id = Hash128::rand();
	state->request->sequence = state->sequence++;
	return 0;
}

int HttpParser::on_headers_complete(http_parser* parser) {
	state_t* state = (state_t*)parser->data;
	switch(parser->method) {
		case ::HTTP_GET: state->request->method = "GET"; break;
		case ::HTTP_HEAD: state->request->method = "HEAD"; break;
		case ::HTTP_POST: state->request->method = "POST"; break;
		case ::HTTP_PUT: state->request->method = "PUT"; break;
	}
	state->request->path = state->url;
	state->request->version = parser->http_major * 10 + parser->http_minor;
	return 0;
}

int HttpParser::on_message_complete(http_parser* parser) {
	state_t* state = (state_t*)parser->data;
	state->complete.push_back(state->request);
	state->request = 0;
	return 0;
}

int HttpParser::on_url(http_parser* parser, const char* at, size_t len) {
	state_t* state = (state_t*)parser->data;
	state->url.append(at, len);
	return 0;
}

int HttpParser::on_header_field(http_parser* parser, const char* at, size_t len) {
	state_t* state = (state_t*)parser->data;
	if(!state->last_was_field) {
		state->request->header.push_back(std::pair<std::string, std::string>());
	}
	state->request->header.back().first.append(at, len);
	state->last_was_field = true;
	return 0;
}

int HttpParser::on_header_value(http_parser* parser, const char* at, size_t len) {
	state_t* state = (state_t*)parser->data;
	state->request->header.back().second.append(at, len);
	state->last_was_field = false;
	return 0;
}

int HttpParser::on_body(http_parser* parser, const char* at, size_t len) {
	state_t* state = (state_t*)parser->data;
	vnx::Memory& payload = state->request->payload;
	::memcpy(payload.add_chunk(len), at, len);
	return 0;
}

void HttpParser::init() {
	subscribe(input);
}

void HttpParser::main() {
	
	::http_parser_settings_init(&settings);
	settings.on_message_begin = &HttpParser::on_message_begin;
	settings.on_headers_complete = &HttpParser::on_headers_complete;
	settings.on_message_complete = &HttpParser::on_message_complete;
	settings.on_url = &HttpParser::on_url;
	settings.on_header_field = &HttpParser::on_header_field;
	settings.on_header_value = &HttpParser::on_header_value;
	settings.on_body = &HttpParser::on_body;
	
	set_timer_millis(100, std::bind(&HttpParser::update, this));
	
	Super::main();
}

void HttpParser::handle(std::shared_ptr<const ::vnx::web::StreamEventArray> events) {
	for(const stream_event_t& event : events->array) {
		switch(event.event) {
			case stream_event_t::EVENT_EOF:
				state_map.erase(event.stream);
				break;
		}
	}
	publish(events, output, BLOCKING);
}

std::shared_ptr<Object> parse_parameter(std::shared_ptr<Object> parameter, const char* str, size_t len) {
	int state = 0;
	std::string key;
	std::string value;
	for(size_t i = 0; i < len; ++i) {
		const char c = str[i];
		if(c == '=' && state == 0) {
			state = 1;
			value.clear();
		} else if(c == '&' && state == 1) {
			if(!parameter) {
				parameter = Object::create();
			}
			vnx::from_string(value, parameter->field[key]);
			state = 0;
			key.clear();
		} else {
			switch(state) {
				case 0: key.push_back(c); break;
				case 1: value.push_back(c); break;
			}
		}
	}
	if(state == 1) {
		if(!parameter) {
			parameter = Object::create();
		}
		vnx::from_string(value, parameter->field[key]);
	}
	return parameter;
}

void HttpParser::handle(std::shared_ptr<const ::vnx::web::StreamRead> input) {
	state_t& state = state_map[input->stream];
	state.stream = input->stream;
	state.parser.data = &state;
	state.size += input->data.size();
	
	if(state.is_closed || state.size > max_request_size) {
		close_stream(state, input->channel);
		return;
	}
	
	const size_t num_parsed = ::http_parser_execute(&state.parser, &settings, (const char*)input->data.data(), input->data.size());
	if(num_parsed != input->data.size()) {
		close_stream(state, input->channel);
		return;
	}
	if(state.parser.upgrade) {
		close_stream(state, input->channel);
		return;
	}
	
	for(const auto& request : state.complete) {
		std::shared_ptr<Object> parameter;
		if(request->method == "GET" || request->method == "POST") {
			if(!request->path.empty()) {
				std::string& str = request->path.back();
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
		}
		if(request->method == "POST") {
			const std::string payload = request->payload.as_string();
			parameter = parse_parameter(parameter, payload.c_str(), payload.size());
		}
		request->stream = input->stream;
		request->parameter = parameter;
		request->source.push_back(input->channel);
		request->time_stamp_ms = vnx::get_time_millis();
		publish(request, output, BLOCKING);
	}
	state.complete.clear();
}

void HttpParser::update() {
	for(const auto& entry : close_map) {
		auto events = StreamEventArray::create();
		for(const auto& stream : entry.second) {
			events->array.push_back(stream_event_t::create(stream, stream_event_t::EVENT_CLOSE));
		}
		if(!events->array.empty()) {
			publish(events, entry.first);
		}
	}
	close_map.clear();
}

void HttpParser::close_stream(state_t& state, const TopicPtr& channel) {
	if(!state.is_closed) {
		close_map[channel].push_back(state.stream);
	}
	state.is_closed = true;
}


} // web
} // vnx
