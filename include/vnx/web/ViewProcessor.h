
#ifndef INCLUDE_VNX_WEB_VIEWPROCESSOR_H
#define INCLUDE_VNX_WEB_VIEWPROCESSOR_H

#include <vnx/web/ViewProcessorBase.hxx>
#include <vnx/web/Provider.hxx>


namespace vnx {
namespace web {

class ViewProcessor : public ViewProcessorBase {
public:
	ViewProcessor(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::Request> request) override;
	
	void handle(std::shared_ptr<const ::vnx::web::Response> response) override;
	
	void update();
	
private:
	std::shared_ptr<Provider> provider;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_VIEWPROCESSOR_H
