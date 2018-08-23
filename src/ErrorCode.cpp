
#include <vnx/web/ErrorCode.hxx>
#include <vnx/Time.h>


namespace vnx {
namespace web {

std::shared_ptr<const ::vnx::web::ErrorCode> ErrorCode::create(const ::int32_t& code) {
	std::shared_ptr<::vnx::web::ErrorCode> value = ErrorCode::create();
	value->code = code;
	value->time_stamp_ms = vnx::get_time_millis();
	return value;
}

std::shared_ptr<const ::vnx::web::ErrorCode> ErrorCode::create_with_message(const ::int32_t& code, const ::std::string& message) {
	std::shared_ptr<::vnx::web::ErrorCode> value = ErrorCode::create();
	value->code = code;
	value->message = message;
	value->time_stamp_ms = vnx::get_time_millis();
	return value;
}

::std::string ErrorCode::get_error_string(const ::int32_t& code) {
	switch(code) {
		case 200: return "OK";
		case 400: return "Bad Request";
		case 404: return "Not Found";
		case 429: return "Timeout";
		case 500: return "Internal Server Error";
		case 503: return "Overload";
		default: return "Unknown";
	}
}


} // web
} // vnx
