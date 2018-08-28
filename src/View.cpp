
#include <vnx/web/View.hxx>


namespace vnx {
namespace web {

void View::require(const ::vnx::web::Path& file) {
	resource[file] = 0;
}

void View::initialize() {
	// nothing here
}

std::shared_ptr<const Value> View::get_resource(const ::vnx::web::Path& file) const {
	auto iter = resource.find(file);
	if(iter != resource.end()) {
		return iter->second;
	}
	return 0;
}

std::shared_ptr<const Request> View::forward(	const std::shared_ptr<const Request>& request,
												const vnx::TopicPtr& channel) const
{
	std::shared_ptr<Request> new_request = Request::create();
	new_request->forward(request, channel);
	return new_request;
}

std::shared_ptr<const Response> View::process(const std::shared_ptr<const Response>& response) const {
	std::shared_ptr<Response> new_response = Response::create();
	new_response->forward(response);
	return response;
}


} // web
} // vnx
