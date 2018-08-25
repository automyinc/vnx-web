
#include <vnx/web/FileSystem.h>
#include <vnx/web/Cache.h>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["n"] = "node";
	options["node"] = "server url";
	options["domain"] = "server domain topic";
	options["input"] = "request input topic";
	
	vnx::init("vnxfilesystem", argc, argv, options);
	
	std::string node = "vnxfilesystem.sock";
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
		vnx::Handle<vnx::web::Cache> module = new vnx::web::Cache("Cache");
		vnx::read_config("input", module->input);
		vnx::read_config("domain", module->domain);
		module->channel = "internal.cache";
		module.start_detached();
	}
	
	{
		vnx::Handle<vnx::web::FileSystem> module = new vnx::web::FileSystem("FileSystem");
		vnx::read_config("domain", module->domain);
		module->input = "internal.filesystem.request";
		module.start_detached();
	}
	
	/*
	 * Wait until shutdown.
	 */
	vnx::wait();
	
}
