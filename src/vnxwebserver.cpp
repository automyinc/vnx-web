
#include <vnx/web/Frontend.h>
#include <vnx/web/HttpParser.h>
#include <vnx/web/HttpProcessor.h>
#include <vnx/web/HttpRenderer.h>
#include <vnx/web/Cache.h>
#include <vnx/web/FileSystem.h>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>

#include <unistd.h>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["n"] = "node";
	options["node"] = "server url";
	
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
		vnx::Handle<vnx::web::Frontend> module = new vnx::web::Frontend("Frontend");
		module->input = "test.frontend.return_data";
		module->channel = "test.frontend.channel";
		module->output = "test.frontend.client_data";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::HttpParser> module = new vnx::web::HttpParser("HttpParser");
		module->input = "test.frontend.client_data";
		module->output = "test.http.request";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::HttpProcessor> module = new vnx::web::HttpProcessor("HttpProcessor");
		module->input = "test.http.request";
		module->channel = "test.http.processor";
		module->output = "test.http.response";
		module->default_domain = "test.com";
		module->domain_map["test.com"] = "test.cache.request";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::HttpRenderer> module = new vnx::web::HttpRenderer("HttpRenderer");
		module->input = "test.http.response";
		module->output = "test.frontend.return_data";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::Cache> module = new vnx::web::Cache("Cache");
		module->domain = "test.domain";
		module->input = "test.cache.request";
		module->channel = "test.cache.channel";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::FileSystem> module = new vnx::web::FileSystem("FileSystem");
		module->domain = "test.domain";
		module->input = "test.filesystem.request";
		module.start_detached();
	}
	
	vnx::wait();
	
}
