
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
	
	vnx::init("vnxfilesystemserver", argc, argv, options);
	
	/*
	 * By default we create a local UNIX node for this example.
	 */
	std::string node = "vnxfilesystemserver.sock";
	vnx::read_config("node", node);
	
	{
		/*
		 * First we start a terminal to display log messages for us.
		 */
		vnx::Handle<vnx::Terminal> terminal = new vnx::Terminal("Terminal");
		terminal.start_detached();
	}
	
	{
		/*
		 * The Server will listen on and accept client connections to this process,
		 * depending on the type of Endpoint either on a TCP port or on a UNIX domain socket.
		 */
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
	
//	vnx::Publisher publisher;
//	while(vnx::do_run()) {
//		std::shared_ptr<vnx::web::Request> request = vnx::web::Request::create();
//		request->id = vnx::Hash128::rand();
//		request->id = request_type_e::READ;
//		request->path = "/LICENSE";
//		request->channel = "test.channel";
//		request->time_stamp_ms = vnx::get_time_millis();
//		publisher.publish(request, "internal.filesystem.request");
//		::usleep(1);
//	}
	
	/*
	 * Wait until shutdown.
	 */
	vnx::wait();
	
}
