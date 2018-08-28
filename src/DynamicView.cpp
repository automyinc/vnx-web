
#include <vnx/web/DynamicView.hxx>
#include <vnx/web/DynamicFile.hxx>


namespace vnx {
namespace web {

std::shared_ptr<const Response> DynamicView::process(const std::shared_ptr<const Response>& response) const {
	
	std::shared_ptr<Response> new_response = Response::create();
	new_response->forward(response);
	
	{
		auto file = std::dynamic_pointer_cast<const File>(response->result);
		if(file) {
			if(file->mime_type.substr(0, 5) == "text/") {
				auto new_file = DynamicFile::create();
				(File&)(*new_file) = *file;
				new_file->parse();
				new_response->result = new_file;
				new_response->time_to_live_ms /= 2;
				return new_response;
			}
		}
	}
	
	return new_response;
}


} // web
} // vnx
