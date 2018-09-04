
#include <vnx/web/Database.h>

#include <vnx/Config.h>
#include <vnx/Process.h>
#include <vnx/Terminal.h>
#include <vnx/Server.h>


int main(int argc, char** argv) {
	
	std::map<std::string, std::string> options;
	options["n"] = "node";
	options["l"] = "location";
	options["node"] = "server url";
	options["name"] = "database name";
	options["location"] = "database location";
	
	vnx::init("vnxdatabase", argc, argv, options);
	
	std::string node = "vnxdatabase.sock";
	vnx::read_config("node", node);
	
	std::string name = "Database";
	vnx::read_config("name", name);
	
	{
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	
	{
		vnx::Handle<vnx::Server> server = new vnx::Server("Server", vnx::Endpoint::from_url(node));
		server.start_detached();
	}
	
	{
		vnx::Handle<vnx::web::Database> module = new vnx::web::Database(name);
		vnx::read_config("location", module->location);
		module.start_detached();
	}
	
	/*
	 * Wait until shutdown.
	 */
	vnx::wait();
	
}
