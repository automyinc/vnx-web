
#ifndef INCLUDE_VNX_WEB_FRONTEND_H
#define INCLUDE_VNX_WEB_FRONTEND_H

#include <vnx/web/FrontendBase.hxx>

#include <unordered_map>


namespace vnx {
namespace web {

class Frontend : public FrontendBase {
public:
	Frontend(const std::string& _vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamEventArray> value) override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamWrite> value) override;
	
private:
	int server_sock = -1;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_FRONTEND_H
