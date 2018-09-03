
#include <vnx/web/DatabaseView.h>
#include <vnx/web/Response.hxx>
#include <vnx/web/File.hxx>
#include <vnx/query/package.h>

#include <sstream>


namespace vnx {
namespace web {

DatabaseView::DatabaseView(const std::string& _vnx_name)
	:	DatabaseViewBase(_vnx_name)
{
}

void DatabaseView::main() {
	
	{
		auto provider = Provider::create();
		provider->id = Hash128::rand();
		provider->path = path;
		provider->input = input;
		this->provider = provider;
	}
	
	client = std::make_shared<database::DatabaseClient>(database);
	
	subscribe(input);
	
	set_timer_millis(500, std::bind(&DatabaseView::update, this));
	
	Super::main();
}

void DatabaseView::render_overview(std::ostream& out) {
	auto info = client->get_table_info();
	out << "<table>\n";
	out << "<tr><th>Table</th><th>Rows</th></tr>\n";
	for(const auto& entry : info) {
		out << "<tr>\n";
		out << "<td><a href=\"table/" << entry.first << "\">" << entry.first << "</a></td>\n";
		out << "<td style=\"text-align:right;\">" << entry.second["num_rows"].to_string() << "</td>\n";
		out << "</tr>\n";
	}
	out << "</table>\n";
}

void DatabaseView::render_table(std::ostream& out, const std::vector<Object>& result, bool hide_id) {
	std::set<std::string> cols;
	for(const auto& row : result) {
		for(const auto& entry : row.field) {
			cols.insert(entry.first);
		}
	}
	if(hide_id) {
		cols.erase("id");
	}
	out << "<table>\n";
	out << "<tr>\n";
	for(const auto& col : cols) {
		out << "<th>" << col << "</th>";
	}
	out << "</tr>";
	for(const auto& row : result) {
		out << "<tr>";
		for(const auto& col : cols) {
			out << "<td>" << row[col] << "</td>";
		}
		out << "</tr>\n";
	}
	out << "</table>\n";
}

void DatabaseView::render_table_index(std::ostream& out, const std::string& table) {
	auto result = client->select(query::select(table, 0, 0, query::limit(1000)));
	render_table(out, result, true);
}

void DatabaseView::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	
	auto response = Response::create();
	response->is_for_request(request);
	
	std::ostringstream out;
	out << "<!DOCTYPE html>\n";
	out << "<html>\n<head>\n";
	out << "<title>DatabaseView of '" << database << "'</title>\n";
	out << "<style>\n";
	out << "table {border-collapse: collapse; width: 100%;}\n";
	out << "th, td {border: 1px solid black;}\n";
	out << "</style>\n";
	out << "</head>\n<body>\n";
	
	try {
		if(request->path.is_root()) {
			render_overview(out);
		} else if(request->path.size() >= 2) {
			if(request->path[1] == "table") {
				if(request->path.size() >= 3) {
					render_table_index(out, request->path[2]);
				}
			}
		}
	} catch(const std::exception& ex) {
		out << "<h2>Error: " << ex.what() << "</h2>";
	}
	
	out << "</body>\n</html>\n";
	
	auto file = File::create();
	file->mime_type = "text/html";
	file->time_stamp_ms = vnx::get_time_millis();
	file->data = out.str();
	response->result = file;
	
	publish(response, request->get_return_channel());
}

void DatabaseView::update() {
	publish(provider, domain);
}


} // web
} // vnx
