
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
	
	Super::main();
}

void ViewProcessor::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	
}

void ViewProcessor::handle(std::shared_ptr<const ::vnx::web::Response> response) {
	
}

void ViewProcessor::update() {
	publish(provider, domain);
}


} // web
} // vnx
