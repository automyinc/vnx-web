
#include <vnx/web/HttpParser.h>

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
	state->request->time_stamp_ms = vnx::get_time_millis();
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
		state->request->header.emplace_back(std::pair<std::string, std::string>());
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
	BinaryData& payload = state->request->payload;
	payload.chunks.emplace_back(vnx::Buffer(len));
	::memcpy(payload.chunks.back().data(), at, len);
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
	
	Super::main();
}

void HttpParser::handle(std::shared_ptr<const ::vnx::web::StreamEvent> event) {
	switch(event->event) {
		case StreamEvent::EVENT_EOF:
			state_map.erase(event->stream);
			break;
	}
	publish(event, output);
}

void HttpParser::handle(std::shared_ptr<const ::vnx::web::StreamRead> input) {
	state_t& state = state_map[input->stream];
	state.parser.data = &state;
	state.size += input->data.size();
	
	if(state.is_error || state.size > max_request_size) {
		send_close(input->stream, input->channel);
		return;
	}
	
	const size_t num_parsed = ::http_parser_execute(&state.parser, &settings, (const char*)input->data.data(), input->data.size());
	if(num_parsed != input->data.size()) {
		state.is_error = true;
		send_close(input->stream, input->channel);
		return;
	}
	if(state.parser.upgrade) {
		state.is_error = true;
		send_close(input->stream, input->channel);
		return;
	}
	
	for(auto request : state.complete) {
		request->stream = input->stream;
		request->channel = input->channel;
		publish(request, output, BLOCKING);
	}
	state.complete.clear();
}

void HttpParser::send_close(const Hash128& stream, TopicPtr channel) {
	auto event = StreamEvent::create();
	event->stream = stream;
	event->event = StreamEvent::EVENT_CLOSE;
	publish(event, channel);
}


} // web
} // vnx