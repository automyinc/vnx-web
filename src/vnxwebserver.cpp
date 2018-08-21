
#include <vnx/web/HttpParser.h>
#include <vnx/web/HttpProcessor.h>
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
		vnx::Handle<vnx::web::HttpParser> module = new vnx::web::HttpParser("HttpParser");
		module->input = "test.stream";
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
		vnx::Handle<vnx::web::Cache> module = new vnx::web::Cache("Cache");
		module->domain = "test.domain";
		module->input = "test.cache.request";
		module->channel = "test.cache.channel";
		module.start_detached();
	}
	{
		vnx::Handle<vnx::web::FileSystem> module = new vnx::web::FileSystem("FileSystem");
		module->domain = "test.domain";
		module->input = "internal.filesystem.request";
		module.start_detached();
	}
	
	const std::string test_request =
		"GET /LICENSE HTTP/1.1\r\n\r\n"
	;
	
	vnx::Hash128 stream = vnx::Hash128::rand();
	vnx::Publisher publisher;
	vnx::Subscriber subscriber;
	subscriber.subscribe("test.server");
	int64_t resume_time = 0;
	while(vnx::do_run()) {
		const int64_t now = vnx::get_time_millis();
		if(now >= resume_time) {
			std::shared_ptr<vnx::web::StreamRead> sample = vnx::web::StreamRead::create();
			sample->stream = stream;
			sample->data = test_request;
			sample->channel = "test.server";
			publisher.publish(sample, "test.stream", vnx::Message::BLOCKING);
		} else {
			::usleep(1);
		}
//		::usleep(1);
		while(auto msg = subscriber.read()) {
			auto sample = std::dynamic_pointer_cast<const vnx::Sample>(msg);
			if(sample) {
				auto value = std::dynamic_pointer_cast<const vnx::web::StreamEventArray>(sample->value);
				if(value) {
					for(const auto& event : value->array) {
						switch(event.event) {
							case vnx::web::stream_event_t::EVENT_PAUSE:
								resume_time = now + event.value;
								break;
							case vnx::web::stream_event_t::EVENT_RESUME:
								resume_time = now;
								break;
						}
					}
				}
			}
		}
	}
	
	vnx::wait();
	
}
