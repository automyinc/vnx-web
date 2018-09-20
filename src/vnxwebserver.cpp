
#include <vnx/web/Frontend.h>
#include <vnx/web/HttpParser.h>
#include <vnx/web/HttpProcessor.h>
#include <vnx/web/HttpRenderer.h>
#include <vnx/web/Cache.h>
#include <vnx/web/FileSystem.h>
#include <vnx/web/ViewProcessor.h>
#include <vnx/web/DefaultView.hxx>
#include <vnx/web/DynamicView.hxx>
#include <vnx/web/PageView.hxx>
#include <vnx/web/DatabaseView.h>
#include <vnx/web/Database.h>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["n"] = "node";
	options["node"] = "server url";
	options["views"] = "map of views";
	
	vnx::init("vnxwebserver", argc, argv, options);
	
	std::string node = "vnxwebserver.sock";
	vnx::read_config("node", node);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("Server", vnx::Endpoint::from_url(node));
		server.start_detached();
	}
	
	{
		vnx::Handle<vnx::web::Database> module = new vnx::web::Database("Database");
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::FileSystem> module = new vnx::web::FileSystem("FileSystem");
		module->domain = "server.domain";
		module->input = "server.filesystem.request";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::Cache> module = new vnx::web::Cache("Cache");
		module->domain = "server.domain";
		module->input = "server.cache.request";
		module->channel = "server.cache.channel";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::ViewProcessor> module = new vnx::web::ViewProcessor("ViewProcessor");
		module->domain = "server.domain";
		module->input = "server.default.request";
		module->channel = "server.default.channel";
		module->output = "server.cache.request";
		{
			auto view = vnx::web::DefaultView::create();
			view->path = "/default/";
			vnx::read_config("DefaultView", view);
			module->view = view;
		}
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::ViewProcessor> module = new vnx::web::ViewProcessor("ViewProcessor");
		module->domain = "server.domain";
		module->input = "server.dynamic.request";
		module->channel = "server.dynamic.channel";
		module->output = "server.cache.request";
		{
			auto view = vnx::web::DynamicView::create();
			view->path = "/dynamic/";
			vnx::read_config("DynamicView", view);
			module->view = view;
		}
		module.start_detached();
	}
	{
		std::map<std::string, std::string> views;
		vnx::read_config("views", views);
		for(const auto& entry : views) {
			const std::string& desc = entry.first;
			std::string type = desc;
			std::string name = desc;
			{
				const size_t pos = desc.find_first_of(':');
				if(pos != std::string::npos) {
					type = desc.substr(0, pos);
					name = desc.substr(pos + 1);
				}
			}
			std::shared_ptr<vnx::web::View> view = std::dynamic_pointer_cast<vnx::web::View>(vnx::create(vnx::Hash64(type)));
			if(!view) {
				view = std::dynamic_pointer_cast<vnx::web::View>(vnx::create(vnx::Hash64("vnx.web." + type)));
			}
			if(view) {
				vnx::from_string(entry.second, view);
				vnx::Handle<vnx::web::ViewProcessor> module = new vnx::web::ViewProcessor("ViewProcessor");
				module->domain = "server.domain";
				module->input = "server." + name + ".request";
				module->channel = "server." + name + ".channel";
				module->output = "server.cache.request";
				module->view = view;
				module.start_detached();
				vnx::log_info().out << "Added view " << desc << " with config " << entry.second;
			} else {
				vnx::log_warn().out << "Unknown view type: " << type;
			}
		}
	}
	{
		vnx::Handle<vnx::web::DatabaseView> module = new vnx::web::DatabaseView("DatabaseView");
		module->domain = "server.domain";
		module->input = "server.database.request";
		module->database = "Database";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::HttpRenderer> module = new vnx::web::HttpRenderer("HttpRenderer");
		module->input = "server.http.response";
		module->output = "server.frontend.return_data";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::HttpProcessor> module = new vnx::web::HttpProcessor("HttpProcessor");
		module->input = "server.http.request";
		module->channel = "server.http.processor";
		module->output = "server.http.response";
		module->domain_map[module->default_domain] = "server.cache.request";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::HttpParser> module = new vnx::web::HttpParser("HttpParser");
		module->input = "server.frontend.client_data";
		module->output = "server.http.request";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::Frontend> module = new vnx::web::Frontend("Frontend");
		module->input = "server.frontend.return_data";
		module->channel = "server.frontend.channel";
		module->output = "server.frontend.client_data";
		module.start_detached();
	}
	
	vnx::wait();
	
}
