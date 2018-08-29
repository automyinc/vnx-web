
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_HttpRequest_HXX_
#define INCLUDE_vnx_web_HttpRequest_HXX_

#include <vnx/web/package.hxx>
#include <vnx/Hash128.h>
#include <vnx/Memory.h>
#include <vnx/web/Request.hxx>


namespace vnx {
namespace web {

class HttpRequest : public ::vnx::web::Request {
public:
	
	::int64_t sequence = 0;
	::std::string method;
	::int32_t version = 0;
	::std::vector<::std::pair<::std::string, ::std::string>> header;
	::vnx::Memory payload;
	
	typedef ::vnx::web::Request Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	static std::shared_ptr<HttpRequest> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	friend std::ostream& operator<<(std::ostream& _out, const HttpRequest& _value);
	friend std::istream& operator>>(std::istream& _in, HttpRequest& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_HttpRequest_HXX_
