
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_query_NotEqual_HXX_
#define INCLUDE_vnx_query_NotEqual_HXX_

#include <vnx/query/package.hxx>
#include <vnx/Object.h>
#include <vnx/Variant.h>
#include <vnx/query/Expression.hxx>


namespace vnx {
namespace query {

class NotEqual : public ::vnx::query::Expression {
public:
	
	::std::shared_ptr<const ::vnx::query::Expression> A;
	::std::shared_ptr<const ::vnx::query::Expression> B;
	
	typedef ::vnx::query::Expression Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	virtual ::vnx::Variant execute(const ::vnx::Object& object) const;
	virtual ::std::string as_string() const;
	
	static std::shared_ptr<NotEqual> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const NotEqual& _value);
	friend std::istream& operator>>(std::istream& _in, NotEqual& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace query

#endif // INCLUDE_vnx_query_NotEqual_HXX_
