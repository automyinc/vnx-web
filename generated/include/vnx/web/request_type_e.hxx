
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_request_type_e_HXX_
#define INCLUDE_vnx_web_request_type_e_HXX_

#include <vnx/Type.h>
#include <vnx/web/package.hxx>


namespace vnx {
namespace web {

struct request_type_e {
	
	enum {
		GENERIC = 3563222431,
		READ = 329112197,
		WRITE = 1285543041,
	};
	
	::uint32_t value = 0;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	request_type_e() {}
	request_type_e(uint32_t _value) { value = _value; }
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	operator uint32_t() const { return value; }
	request_type_e& operator=(uint32_t _value) { value = _value; return *this; }
	
	static std::shared_ptr<request_type_e> create();
	std::shared_ptr<request_type_e> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const request_type_e& _value);
	friend std::istream& operator>>(std::istream& _in, request_type_e& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_request_type_e_HXX_
