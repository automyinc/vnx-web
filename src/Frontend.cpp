
#include <vnx/web/Frontend.h>


namespace vnx {
namespace web {

Frontend::Frontend(const std::string& _vnx_name)
	:	FrontendBase(_vnx_name)
{
}

void Frontend::init() {
	subscribe(channel, 0);
}

void Frontend::main() {
	
	
	
	Super::main();
}

void Frontend::handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) {
	
}

void Frontend::handle(std::shared_ptr<const ::vnx::web::StreamWrite> value) {
	
}


} // web
} // vnx
