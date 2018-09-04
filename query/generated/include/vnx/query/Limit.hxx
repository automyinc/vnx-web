
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_query_Limit_HXX_
#define INCLUDE_vnx_query_Limit_HXX_

#include <vnx/query/package.hxx>
#include <vnx/Value.h>


namespace vnx {
namespace query {

class Limit : public ::vnx::Value {
public:
	
	::int64_t num_rows = -1;
	::int64_t offset = 0;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	virtual ::std::string as_string() const;
	
	static std::shared_ptr<Limit> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const Limit& _value);
	friend std::istream& operator>>(std::istream& _in, Limit& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace query

#endif // INCLUDE_vnx_query_Limit_HXX_
