
#include <vnx/web/ViewProcessor.h>


namespace vnx {
namespace web {

ViewProcessor::ViewProcessor(const std::string& _vnx_name)
	:	ViewProcessorBase(_vnx_name)
{
}

void ViewProcessor::main() {
	
	if(!view) {
		log(ERROR).out << "view == null";
		return;
	}
	view->initialize();
	
	{
		auto provider = Provider::create();
		provider->id = Hash128::rand();
		provider->path = view->path;
		provider->input = input;
		this->provider = provider;
	}
	
	subscribe(input, max_input_queue_ms);
	subscribe(channel);
	
	set_timer_millis(render_interval_ms, std::bind(&ViewProcessor::render, this));
	set_timer_millis(update_interval_ms, std::bind(&ViewProcessor::update, this));
	
	render();
	
	Super::main();
}

void ViewProcessor::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	auto forward = view->forward(request, channel);
	if(forward) {
		publish(forward, output);
	}
}

void ViewProcessor::handle(std::shared_ptr<const ::vnx::web::Response> response) {
	{
		auto iter = pending_resource.find(response->id);
		if(iter != pending_resource.end()) {
			view->resource[iter->second] = response->result;
			pending_resource.erase(response->id);
			return;
		}
	}
	auto forward = view->process(response);
	if(forward) {
		publish(forward, response->get_return_channel());
	}
}

void ViewProcessor::render() {
	for(const auto& entry : view->resource) {
		auto request = Request::create();
		request->initialize(request_type_e::READ, entry.first, channel, render_interval_ms);
		pending_resource[request->id] = entry.first;
		publish(request, output, BLOCKING);
	}
}

void ViewProcessor::update() {
	publish(provider, domain);
}


} // web
} // vnx
