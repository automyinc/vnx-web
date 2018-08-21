
#ifndef INCLUDE_VNX_WEB_HTTPPARSER_H
#define INCLUDE_VNX_WEB_HTTPPARSER_H

#include <vnx/web/HttpParserBase.hxx>
#include <vnx/web/HttpRequest.hxx>
#include <vnx/web/BinaryData.hxx>

#include <http-parser/http_parser.h>

#include <unordered_map>


namespace vnx {
namespace web {

class HttpParser : public HttpParserBase {
public:
	HttpParser(const std::string& _vnx_name);

protected:
	struct state_t {
		state_t();
		Hash128 stream;
		::http_parser parser;
		int64_t sequence = 1;
		size_t size = 0;
		std::string url;
		std::shared_ptr<HttpRequest> request;
		std::vector<std::shared_ptr<HttpRequest>> complete;
		bool last_was_field = false;
		bool is_closed = false;
	};
	
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamRead> input) override;
	
private:
	static int on_message_begin(http_parser* parser);
	static int on_headers_complete(http_parser* parser);
	static int on_message_complete(http_parser* parser);
	static int on_url(http_parser* parser, const char* at, size_t len);
	static int on_header_field(http_parser* parser, const char* at, size_t len);
	static int on_header_value(http_parser* parser, const char* at, size_t len);
	static int on_body(http_parser* parser, const char* at, size_t len);
	
	void update();
	
	void close_stream(state_t& state, const TopicPtr& channel);
	
private:
	::http_parser_settings settings;
	std::unordered_map<Hash128, state_t> state_map;
	std::unordered_map<TopicPtr, std::vector<Hash128>> close_map;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_HTTPPARSER_H
