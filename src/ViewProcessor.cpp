
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
	
	{
		auto provider = Provider::create();
		provider->id = Hash128::rand();
		provider->path = view->path;
		provider->input = input;
		this->provider = provider;
	}
	
	subscribe(input);
	subscribe(channel);
	
	set_timer_millis(update_interval_ms, std::bind(&ViewProcessor::update, this));
	
	Super::main();
}

void ViewProcessor::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	publish(view->forward(request, channel), output);
}

void ViewProcessor::handle(std::shared_ptr<const ::vnx::web::Response> response) {
	publish(view->process(response), response->get_return_channel());
}

void ViewProcessor::update() {
	publish(provider, domain);
}


} // web
} // vnx
