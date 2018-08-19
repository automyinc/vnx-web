
#include <vnx/web/Response.hxx>


namespace vnx {
namespace web {

std::shared_ptr<const Response> Response::create(
		const ::std::shared_ptr<const ::vnx::web::Request>& request,
		const ::std::shared_ptr<const ::vnx::web::Content>& content)
{
	std::shared_ptr<Response> response = Response::create();
	response->id = request->id;
	response->content = content;
	return response;
}


} // web
} // vnx
