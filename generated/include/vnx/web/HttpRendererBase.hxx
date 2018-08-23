
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_HttpRendererBase_HXX_
#define INCLUDE_vnx_web_HttpRendererBase_HXX_

#include <vnx/web/package.hxx>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/web/HttpResponse.hxx>
#include <vnx/web/StreamEventArray.hxx>


namespace vnx {
namespace web {

class HttpRendererBase : public ::vnx::Module {
public:
	
	::vnx::TopicPtr input;
	::vnx::TopicPtr output;
	
	typedef ::vnx::Module Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	HttpRendererBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	friend std::ostream& operator<<(std::ostream& _out, const HttpRendererBase& _value);
	friend std::istream& operator>>(std::istream& _in, HttpRendererBase& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
protected:
	virtual void handle(std::shared_ptr<const ::vnx::web::HttpResponse> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::web::HttpResponse> _value) {}
	virtual void handle(std::shared_ptr<const ::vnx::web::StreamEventArray> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::web::StreamEventArray> _value) {}
	
	void handle_switch(std::shared_ptr<const ::vnx::Sample> _sample);
	bool call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type);
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_HttpRendererBase_HXX_