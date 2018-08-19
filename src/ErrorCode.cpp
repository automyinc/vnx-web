
#include <vnx/web/ErrorCode.hxx>


namespace vnx {
namespace web {

std::shared_ptr<const ::vnx::web::ErrorCode> ErrorCode::create(const ::int32_t& code_) {
	std::shared_ptr<::vnx::web::ErrorCode> value = ErrorCode::create();
	value->code = code_;
	return value;
}


} // web
} // vnx
