
#include <vnx/web/View.hxx>


namespace vnx {
namespace web {

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
