
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_ErrorCode_HXX_
#define INCLUDE_vnx_web_ErrorCode_HXX_

#include <vnx/web/package.hxx>
#include <vnx/Value.h>


namespace vnx {
namespace web {

class ErrorCode : public ::vnx::Value {
public:
	static const ::int32_t MOVED_PERMANENTLY = 301;
	static const ::int32_t BAD_REQUEST = 400;
	static const ::int32_t FORBIDDEN = 403;
	static const ::int32_t NOT_FOUND = 404;
	static const ::int32_t TIMEOUT = 429;
	static const ::int32_t INTERNAL_ERROR = 500;
	static const ::int32_t OVERLOAD = 503;
	
	::int32_t code = 0;
	::std::string message;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	static ::std::shared_ptr<const ::vnx::web::ErrorCode> create(const ::int32_t& code);
	static ::std::shared_ptr<const ::vnx::web::ErrorCode> create_with_message(const ::int32_t& code, const ::std::string& message);
	static ::std::string get_error_string(const ::int32_t& code);
	
	static std::shared_ptr<ErrorCode> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	friend std::ostream& operator<<(std::ostream& _out, const ErrorCode& _value);
	friend std::istream& operator>>(std::istream& _in, ErrorCode& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_ErrorCode_HXX_
