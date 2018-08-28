
#include <vnx/web/Request.hxx>


namespace vnx {
namespace web {

::vnx::TopicPtr Request::get_return_channel() const {
	if(source.empty()) {
		return nullptr;
	}
	return source.back();
}

void Request::initialize(	const ::vnx::web::request_type_e& type,
							const ::vnx::web::Path& path,
							const ::vnx::TopicPtr& source,
							const ::int64_t& timeout_ms)
{
	this->id = Hash128::rand();
	this->type = type;
	this->path = path;
	this->source.push_back(source);
	this->timeout_ms = timeout_ms;
	this->time_stamp_ms = vnx::get_time_millis();
}

void Request::forward(	const ::std::shared_ptr<const ::vnx::web::Request>& request,
						const ::vnx::TopicPtr& channel)
{
	*this = *request;
	id.B() = Hash64(this->id.B().value + 1);
	source.push_back(channel);
}

void Request::forward_relative(	const ::std::shared_ptr<const ::vnx::web::Request>& request,
								const ::vnx::TopicPtr& channel,
								const ::vnx::web::Path& base_path)
{
	forward(request, channel);
	path = request->path.get_sub_path(base_path);
}


} // web
} // vnx
