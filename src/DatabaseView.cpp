
#include <vnx/web/DatabaseView.h>
#include <vnx/web/Response.hxx>
#include <vnx/web/File.hxx>
#include <vnx/web/SetCookie.hxx>
#include <vnx/query/package.h>

#include <sstream>


namespace vnx {
namespace web {

DatabaseView::DatabaseView(const std::string& _vnx_name)
	:	DatabaseViewBase(_vnx_name)
{
}

void DatabaseView::main() {
	
	if(password.size() < 6) {
		log(ERROR).out << "Password too short!";
		return;
	}
	
	{
		auto provider = Provider::create();
		provider->id = Hash128::rand();
		provider->path = path;
		provider->input = input;
		this->provider = provider;
	}
	
	client = std::make_shared<DatabaseClient>(database);
	
	ssid_key = "vnx-4fbe2728-" + database;
	
	subscribe(input);
	
	set_timer_millis(500, std::bind(&DatabaseView::update, this));
	set_timer_millis(5000, std::bind(&DatabaseView::maintain, this));
	
	Super::main();
}

void DatabaseView::render_overview(std::ostream& out) const {
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

void DatabaseView::render_table(	std::ostream& out,
									const std::vector<Object>& result,
									const std::vector<std::string>& fields,
									const std::map<std::string, std::string>& field_header) const
{
	out << "<table>\n";
	out << "<tr>\n";
	for(const auto& col : fields) {
		auto iter = field_header.find(col);
		if(iter != field_header.end()) {
			out << "<th>" << iter->second << "</th>";
		} else {
			out << "<th>" << col << "</th>";
		}
	}
	out << "</tr>\n";
	size_t i = 0;
	for(const auto& row : result) {
		out << "<tr class=\"" << (i++ % 2 ? "tr21" : "tr20") << "\">";
		for(const auto& col : fields) {
			out << "<td>" << row[col] << "</td>";
		}
		out << "</tr>\n";
	}
	out << "</table>\n";
}

void DatabaseView::render_table_index(std::ostream& out, const std::string& table, const Object& parameter) const {
	
	auto query = query::select(table, 0, 0, query::limit(100));
	
	std::map<std::string, std::string> field_header;
	
	for(const auto& entry : parameter.field) {
		if(entry.first.substr(0, 3) == "_A_" && !entry.second.empty()) {
			const std::string field = entry.first.substr(3);
			const std::string func = entry.second.to_string_value();
			if(func == "SUM") {
				query.aggregates[field] = query::sum(query::field(field));
				field_header[field] = "SUM(" + field + ")";
			}
			if(func == "MIN") {
				query.aggregates[field] = query::min(query::field(field));
				field_header[field] = "MIN(" + field + ")";
			}
			if(func == "MAX") {
				query.aggregates[field] = query::max(query::field(field));
				field_header[field] = "MAX(" + field + ")";
			}
			if(func == "AVG") {
				query.aggregates[field] = query::avg(query::field(field));
				field_header[field] = "AVG(" + field + ")";
			}
		}
	}
	{
		const std::string field = parameter["group_by"].to_string_value();
		if(!field.empty()) {
			query.group_by = query::group_by(field);
			query.aggregates["_N_"] = query::count();
			field_header[field] = "GROUP(" + field + ")";
		}
	}
	{
		const std::string field = parameter["order_by"].to_string_value();
		if(!field.empty()) {
			const auto mode = parameter["order_mode"].to<int32_t>();
			query.order_by = query::order_by(field, mode);
			switch(mode) {
				case query::OrderBy::ASC: field_header[field] = field + " (ASC)"; break;
				case query::OrderBy::DESC: field_header[field] = field + " (DESC)"; break;
			}
		}
	}
	{
		const auto num_rows = parameter["limit"].to<int64_t>();
		const auto offset = parameter["offset"].to<int64_t>();
		if(num_rows > 0 || offset > 0) {
			query.limit = query::limit(num_rows, offset);
		}
	}
	
	const auto result = client->select(query);
	
	std::set<std::string> set;
	for(const auto& row : result) {
		for(const auto& entry : row.field) {
			set.insert(entry.first);
		}
	}
	set.erase("id");
	const std::vector<std::string> fields(set.begin(), set.end());
	
	out << "<a href=\"..\">Overview</a>\n";
	out << "<a href=\"?\">Reset</a>\n";
	out << "<div class=\"right\">\n";
	out << "<a href=\"" << path.to_string() << "logout\">Logout</a>";
	out << "</div>\n";
	out << "<hr>\n";
	out << "<form method=\"get\">\n";
	for(const auto& col : fields) {
		out << "<select name=\"_A_" << col << "\">\n";
		out << "<option value=\"\" selected></option>\n";
		out << "<option value=\"SUM\">SUM(" << col << ")</option>\n";
		out << "<option value=\"MIN\">MIN(" << col << ")</option>\n";
		out << "<option value=\"MAX\">MAX(" << col << ")</option>\n";
		out << "<option value=\"AVG\">AVG(" << col << ")</option>\n";
		out << "</select>\n";
	}
	out << " GROUP BY <select name=\"group_by\">\n";
	out << "<option value=\"\" selected></option>\n";
	for(const auto& col : fields) {
		out << "<option value=\"" << col << "\">" << col << "</option>\n";
	}
	out << "</select>\n";
	out << " ORDER BY <select name=\"order_by\">\n";
	out << "<option value=\"\" selected></option>\n";
	for(const auto& col : fields) {
		out << "<option value=\"" << col << "\">" << col << "</option>\n";
	}
	out << "<option value=\"_N_\">_N_</option>\n";
	out << "</select>\n";
	out << "<select name=\"order_mode\">\n";
	out << "<option value=\"" << query::OrderBy::ASC << "\" selected>ASC</option>\n";
	out << "<option value=\"" << query::OrderBy::DESC << "\">DESC</option>\n";
	out << "</select>\n";
	out << " LIMIT <input type=\"text\" name=\"limit\" value=\"100\" align=\"right\" size=\"3\">\n";
	out << " OFFSET <input type=\"text\" name=\"offset\" value=\"0\" align=\"right\" size=\"3\">\n";
	out << "<input type=\"submit\">\n";
	out << "</form>\n<hr>\n";
	out << query.as_string() << std::endl;
	out << "<hr>\n";
	
	render_table(out, result, fields, field_header);
}

std::shared_ptr<Content> DatabaseView::render(	std::shared_ptr<const Request> request,
												std::shared_ptr<const Object> parameter) const
{
	std::ostringstream out;
	out << "<!DOCTYPE html>\n";
	out << "<html>\n<head>\n";
	out << "<title>DatabaseView of '" << database << "'</title>\n";
	out << "<style>\n";
	out << "table {border-collapse: collapse; width: 100%;}\n";
	out << "th, td {border: 1px solid black;}\n";
	out << ".tr20 {background: #eeeeee;}\n";
	out << ".tr21 {background: #ffffff;}\n";
	out << ".right {text-align: right; float: right;}\n";
	out << "</style>\n";
	out << "</head>\n<body>\n";
	
	try {
		if(request->path.is_root()) {
			render_overview(out);
		} else if(request->path.size() >= 2) {
			if(request->path[1] == "table") {
				if(request->path.size() >= 3) {
					render_table_index(out, request->path[2], parameter ? *parameter : Object());
				}
			}
			if(request->path[1] == "logout") {
				current->is_valid = false;
				out << "<h2>Logged out.</h2>\n";
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
	return file;
}

std::shared_ptr<Content> DatabaseView::login(	std::shared_ptr<const Request> request,
												std::shared_ptr<const Object> parameter) const
{
	std::ostringstream out;
	out << "<!DOCTYPE html>\n";
	out << "<html>\n<head>\n";
	out << "<title>Login</title>\n";
	out << "<style>\n";
	out << "</style>\n";
	out << "</head>\n<body>\n";
	
	out << "<h2>Login to '" << database << "'</h2>\n";
	
	out << "<form method=\"post\">\n";
	out << "Password:<input type=\"password\" name=\"password\">\n";
	out << "<input type=\"submit\">\n";
	out << "</form>\n";
	
	if(parameter && parameter->field.count("password")) {
		out << "<br>Error: Wrong password!<br>\n";
	}
	
	out << "</body>\n</html>\n";
	
	auto file = File::create();
	file->mime_type = "text/html";
	file->time_stamp_ms = vnx::get_time_millis();
	file->data = out.str();
	return file;
}

void DatabaseView::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	
	auto response = Response::create();
	response->is_for_request(request);
	
	current = 0;
	{
		auto iter = request->session.find(ssid_key);
		if(iter != request->session.end()) {
			auto it = sessions.find(iter->second);
			if(it != sessions.end()) {
				current = &it->second;
			}
		}
	}
	
	auto parameter = std::dynamic_pointer_cast<const Object>(request->parameter);
	
	if(current) {
		if(current->is_valid) {
			response->result = render(request, parameter);
		} else {
			if(parameter && (*parameter)["password"].to_string_value() == password) {
				current->is_valid = true;
				response->result = ErrorCode::create_with_message(ErrorCode::MOVED_TEMPORARILY, path.to_string());
			} else {
				response->result = login(request, parameter);
			}
		}
	} else {
		const std::string ssid = Hash128::rand().to_hex_string();
		session_t& session = sessions[ssid];
		session.create_time = vnx::get_time_millis();
		response->result = SetCookie::create(ssid_key, ssid, path.to_string() + "login");
	}
	
	publish(response, request->get_return_channel());
}

void DatabaseView::update() {
	publish(provider, domain);
}

void DatabaseView::maintain() {
	std::vector<std::string> list;
	const int64_t now = vnx::get_time_millis();
	for(const auto& entry : sessions) {
		if(!entry.second.is_valid
			&& now - entry.second.create_time > 60*1000)
		{
			list.push_back(entry.first);
		}
	}
	for(const auto& ssid : list) {
		sessions.erase(ssid);
	}
}


} // web
} // vnx
