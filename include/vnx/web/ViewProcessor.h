
#ifndef INCLUDE_VNX_WEB_VIEWPROCESSOR_H
#define INCLUDE_VNX_WEB_VIEWPROCESSOR_H

#include <vnx/web/ViewProcessorBase.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/web/View.hxx>


namespace vnx {
namespace web {

class ViewProcessor : public ViewProcessorBase {
public:
	ViewProcessor(const std::string& _vnx_name);
	
	std::shared_ptr<View> view;
	
protected:
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::Request> request) override;
	
	void handle(std::shared_ptr<const ::vnx::web::Response> response) override;
	
	void render();
	
	void update();
	
private:
	std::shared_ptr<Provider> provider;
	std::unordered_map<Hash128, Path> pending_resource;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_VIEWPROCESSOR_H
