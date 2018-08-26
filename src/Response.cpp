
#include <vnx/web/Response.hxx>


namespace vnx {
namespace web {

::vnx::TopicPtr Response::get_return_channel() const {
	if(!destination.empty()) {
		return destination.back();
	}
	return nullptr;
}

void Response::is_for_request(const ::std::shared_ptr<const ::vnx::web::Request>& request) {
	this->id = request->id;
	this->destination = request->source;
	if(!this->destination.empty()) {
		this->destination.pop_back();
	}
}

void Response::forward(const ::std::shared_ptr<const ::vnx::web::Response>& response) {
	*this = *response;
	if(!this->destination.empty()) {
		this->destination.pop_back();
	}
}

std::shared_ptr<const Response> Response::create(
		const ::std::shared_ptr<const ::vnx::web::Request>& request,
		const ::std::shared_ptr<const ::vnx::web::Content>& content)
{
	std::shared_ptr<Response> response = Response::create();
	response->is_for_request(request);
	response->content = content;
	return response;
}


} // web
} // vnx
