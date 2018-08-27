
#include <vnx/web/DefaultView.hxx>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/Directory.hxx>
#include <vnx/web/File.hxx>
#include <vnx/PrettyPrinter.h>

#include <sstream>
#include <algorithm>


namespace vnx {
namespace web {

std::ostream& render(std::ostream& out, std::shared_ptr<const Directory> directory) {
	out << "<html>\n<body>\n";
	out << "<table>\n";
	if(!directory->is_root) {
		out << "<tr><td><a href=\"..\">..</a></td></tr>\n";
	}
	std::vector<FileInfo> list = directory->files;
	std::sort(list.begin(), list.end(), [](const FileInfo& A, const FileInfo& B) -> bool {
		return A.is_directory ? (B.is_directory ? A.name < B.name : true) : (B.is_directory ? false : A.name < B.name);
	});
	for(const FileInfo& file : list) {
		out << "<tr>\n";
		out << "<td><a href=\"" << file.name << (file.is_directory ? "/" : "") << "\">" << file.name << (file.is_directory ? "/" : "") << "</a></td>\n";
		out << "<td style=\"padding-left:50px;\">" << (file.is_directory ? "" : file.mime_type) << "</td>\n";
		out << "<td style=\"padding-left:50px;text-align:right;\">";
		if(!file.is_directory) {
			if(file.num_bytes < 10 * 1024) {
				out << std::to_string(file.num_bytes) << " B";
			} else if(file.num_bytes < 10 * 1024 * 1024) {
				out << std::to_string(file.num_bytes / 1024) << " K";
			} else {
				out << std::to_string(file.num_bytes / 1024 / 1024) << " M";
			}
		}
		out << "</td>\n</tr>\n";
	}
	out << "</table>\n";
	out << "</body>\n</html>\n";
}

std::shared_ptr<const Response> DefaultView::process(const std::shared_ptr<const Response>& response) const {
	
	std::shared_ptr<Response> new_response = Response::create();
	new_response->forward(response);
	
	{
		auto directory = std::dynamic_pointer_cast<const Directory>(response->result);
		if(directory) {
			if(allow_directory_index) {
				std::ostringstream stream;
				render(stream, directory);
				
				auto file = File::create();
				file->mime_type = "text/html";
				file->time_stamp_ms = directory->time_stamp_ms;
				file->data = stream.str();
				new_response->result = file;
			} else {
				new_response->result = ErrorCode::create(ErrorCode::FORBIDDEN);
			}
			new_response->time_to_live_ms /= 2;
			return new_response;
		}
	}
	{
		auto file = std::dynamic_pointer_cast<const File>(response->result);
		if(file) {
			if(file->mime_type == "application/octet-stream") {
				try {
					vnx::BufferInputStream stream(&file->data);
					vnx::TypeInput in(&stream);
					uint16_t code[2] = {};
					vnx::read(in, code[0]);
					vnx::read(in, code[1]);
					if(code[0] == vnx::CODE_NONE && code[1] == vnx::CODE_MAGIC) {
						std::ostringstream json;
						vnx::PrettyPrinter printer(json);
						vnx::accept(printer, in);
						
						auto new_file = File::create();
						new_file->name = file->name;
						new_file->mime_type = "application/json";
						new_file->time_stamp_ms = file->time_stamp_ms;
						new_file->data = json.str();
						new_response->result = new_file;
						new_response->time_to_live_ms /= 2;
						return new_response;
					}
				} catch(...) {
					// just ignore
				}
			}
		}
	}
	
	return new_response;
}


} // web
} // vnx
