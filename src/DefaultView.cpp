
#include <vnx/web/DefaultView.hxx>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/Directory.hxx>
#include <vnx/web/File.hxx>

#include <sstream>


namespace vnx {
namespace web {

std::ostream& render(std::ostream& out, std::shared_ptr<const Directory> directory) {
	out << "<html>\n<body>\n";
	if(!directory->is_root) {
		out << "<a href=\"..\">..</a><br>\n";
	}
	for(const FileInfo& file : directory->files) {
		out << "<a href=\"" << file.name << (file.is_directory ? "/" : "") << "\">" << file.name << (file.is_directory ? "/" : "") << "</a><br>\n";
	}
	out << "</body>\n</html>\n";
}

std::shared_ptr<const Response> DefaultView::process(const std::shared_ptr<const Response>& response) const {
	
	std::shared_ptr<Response> new_response = Response::create();
	new_response->forward(response);
	
	{
		auto directory = std::dynamic_pointer_cast<const Directory>(response->content);
		if(directory) {
			if(allow_directory_index) {
				std::ostringstream stream;
				render(stream, directory);
				auto file = File::create();
				file->mime_type = "text/html";
				file->time_stamp_ms = directory->time_stamp_ms;
				file->data = stream.str();
				new_response->content = file;
			} else {
				new_response->content = ErrorCode::create(ErrorCode::FORBIDDEN);
			}
			new_response->time_to_live_ms /= 2;
		}
	}
	
	return new_response;
}


} // web
} // vnx
