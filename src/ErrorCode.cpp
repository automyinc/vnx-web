
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


} // web
} // vnx
