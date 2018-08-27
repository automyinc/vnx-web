
#include <vnx/web/Request.hxx>


namespace vnx {
namespace web {

::vnx::TopicPtr Request::get_return_channel() const {
	if(source.empty()) {
		return nullptr;
	}
	return source.back();
}

void Request::forward(	const ::std::shared_ptr<const ::vnx::web::Request>& request,
						const ::vnx::TopicPtr& channel)
{
	*this = *request;
	this->id.B() = Hash64(this->id.B().value + 1);
	this->source.push_back(channel);
}

void Request::forward_relative(	const ::std::shared_ptr<const ::vnx::web::Request>& request,
								const ::vnx::TopicPtr& channel,
								const ::vnx::web::Path& base_path)
{
	forward(request, channel);
	this->path = request->path.get_relative_path(base_path);
}


} // web
} // vnx
