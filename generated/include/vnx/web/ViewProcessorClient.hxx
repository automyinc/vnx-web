
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_ViewProcessor_CLIENT_HXX_
#define INCLUDE_vnx_web_ViewProcessor_CLIENT_HXX_

#include <vnx/Client.h>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/web/HttpRequest.hxx>
#include <vnx/web/HttpResponse.hxx>
#include <vnx/web/Request.hxx>
#include <vnx/web/Response.hxx>
#include <vnx/web/View.hxx>


namespace vnx {
namespace web {

class ViewProcessorClient : public vnx::Client {
public:
	ViewProcessorClient(const std::string& service_name);
	
	ViewProcessorClient(vnx::Hash64 service_addr);
	
	void handle(const ::std::shared_ptr<const ::vnx::web::Request>& sample);
	
	void handle_async(const ::std::shared_ptr<const ::vnx::web::Request>& sample);
	
	void handle(const ::std::shared_ptr<const ::vnx::web::Response>& sample);
	
	void handle_async(const ::std::shared_ptr<const ::vnx::web::Response>& sample);
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_ViewProcessor_CLIENT_HXX_