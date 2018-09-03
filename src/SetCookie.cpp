
#include <vnx/web/SetCookie.hxx>


namespace vnx {
namespace web {

::std::shared_ptr<const ::vnx::web::SetCookie> SetCookie::create(const ::std::string& name, const ::std::string& value, const ::std::string& forward) {
	auto cookie = SetCookie::create();
	cookie->code = ErrorCode::MOVED_TEMPORARILY;
	cookie->message = forward;
	cookie->name = name;
	cookie->value = value;
	return cookie;
}


} // web
} // vnx
