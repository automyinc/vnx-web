
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_Request_HXX_
#define INCLUDE_vnx_web_Request_HXX_

#include <vnx/web/package.hxx>
#include <vnx/Hash128.h>
#include <vnx/TopicPtr.h>
#include <vnx/Value.h>
#include <vnx/web/Path.h>
#include <vnx/web/request_type_e.hxx>


namespace vnx {
namespace web {

class Request : public ::vnx::Value {
public:
	
	::vnx::Hash128 id;
	::vnx::Hash128 stream;
	::vnx::web::request_type_e type = ::vnx::web::request_type_e::GENERIC;
	::vnx::web::Path path;
	::std::shared_ptr<const ::vnx::Value> parameter;
	::std::map<::std::string, ::std::string> session;
	::std::vector<::vnx::TopicPtr> source;
	::int64_t time_stamp_ms = 0;
	::int64_t timeout_ms = 3000;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	virtual ::vnx::TopicPtr get_return_channel() const;
	virtual void initialize(const ::vnx::web::request_type_e& type, const ::vnx::web::Path& path, const ::vnx::TopicPtr& source, const ::int64_t& timeout_ms);
	virtual void forward(const ::std::shared_ptr<const ::vnx::web::Request>& request, const ::vnx::TopicPtr& channel);
	virtual void forward_relative(const ::std::shared_ptr<const ::vnx::web::Request>& request, const ::vnx::TopicPtr& channel, const ::vnx::web::Path& base_path);
	
	static std::shared_ptr<Request> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const Request& _value);
	friend std::istream& operator>>(std::istream& _in, Request& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_Request_HXX_
