
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_SetCookie_HXX_
#define INCLUDE_vnx_web_SetCookie_HXX_

#include <vnx/web/package.hxx>
#include <vnx/web/ErrorCode.hxx>


namespace vnx {
namespace web {

class SetCookie : public ::vnx::web::ErrorCode {
public:
	
	::std::string name;
	::std::string value;
	
	typedef ::vnx::web::ErrorCode Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	static ::std::shared_ptr<const ::vnx::web::SetCookie> create(const ::std::string& name, const ::std::string& value, const ::std::string& forward);
	
	static std::shared_ptr<SetCookie> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const SetCookie& _value);
	friend std::istream& operator>>(std::istream& _in, SetCookie& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_SetCookie_HXX_
