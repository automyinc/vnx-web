
#include <vnx/web/PageView.hxx>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/DynamicFile.hxx>

#include <sstream>


namespace vnx {
namespace web {

Path get_index_path(const PageView* view) {
	if(view->index_path.is_relative()) {
		return view->source_path + view->index_path;
	} else {
		return view->index_path;
	}
}

void PageView::initialize() {
	require(get_index_path(this));
}

std::shared_ptr<const Request> PageView::forward(	const std::shared_ptr<const Request>& request,
													const vnx::TopicPtr& channel) const
{
	std::shared_ptr<Request> new_request = Request::create();
	new_request->forward(request, channel);
	new_request->path = source_path + request->path;
	return new_request;
}

std::shared_ptr<const Response> PageView::process(const std::shared_ptr<const Response>& response) const {
	
	std::shared_ptr<Response> new_response = Response::create();
	new_response->forward(response);
	
	auto index_file = std::dynamic_pointer_cast<const DynamicFile>(get_resource(get_index_path(this)));
	if(index_file) {
		Object context = index_file->static_context;
		auto page_file = std::dynamic_pointer_cast<const DynamicFile>(response->result);
		if(page_file) {
			for(const auto& field : page_file->static_context.field) {
				context[field.first] = field.second;
			}
			auto content = page_file->execute_with_options(index_file->static_context, true);
			context["vnx.web.page.source"].assign((const char*)content->data.data(), content->data.size());
			new_response->time_to_live_ms /= 2;
		} else {
			context["vnx.web.page.source"] = "Page not found!";
			new_response->time_to_live_ms = 1000;
		}
		new_response->result = index_file->execute(context);
	} else {
		new_response->time_to_live_ms = 1000;
	}
	
	new_response->is_dynamic = false;
	return new_response;
}


} // web
} // vnx
