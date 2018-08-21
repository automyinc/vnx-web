
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_CacheBase_HXX_
#define INCLUDE_vnx_web_CacheBase_HXX_

#include <vnx/web/package.hxx>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/web/Content.hxx>
#include <vnx/web/Directory.hxx>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/File.hxx>
#include <vnx/web/HttpRequest.hxx>
#include <vnx/web/HttpResponse.hxx>
#include <vnx/web/ObjectFile.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/web/Request.hxx>
#include <vnx/web/Response.hxx>
#include <vnx/web/TextFile.hxx>
#include <vnx/web/ValueFile.hxx>


namespace vnx {
namespace web {

class CacheBase : public ::vnx::Module {
public:
	
	::vnx::TopicPtr domain;
	::vnx::TopicPtr input;
	::vnx::TopicPtr channel;
	::int64_t num_entries = 1024;
	::int64_t max_entry_size = 4194304;
	::int64_t max_pending = 10000;
	::int32_t update_interval_ms = 500;
	::int32_t maintain_interval_ms = 5000;
	
	typedef ::vnx::Module Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	CacheBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	friend std::ostream& operator<<(std::ostream& _out, const CacheBase& _value);
	friend std::istream& operator>>(std::istream& _in, CacheBase& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
protected:
	virtual void handle(std::shared_ptr<const ::vnx::web::Content> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::web::Content> _value) {}
	virtual void handle(std::shared_ptr<const ::vnx::web::Provider> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::web::Provider> _value) {}
	virtual void handle(std::shared_ptr<const ::vnx::web::Request> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::web::Request> _value) {}
	virtual void handle(std::shared_ptr<const ::vnx::web::Response> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::web::Response> _value) {}
	virtual void purge() = 0;
	
	void handle_switch(std::shared_ptr<const ::vnx::Sample> _sample);
	bool call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type);
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_CacheBase_HXX_
