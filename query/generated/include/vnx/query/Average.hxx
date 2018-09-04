
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_query_Average_HXX_
#define INCLUDE_vnx_query_Average_HXX_

#include <vnx/query/package.hxx>
#include <vnx/Object.h>
#include <vnx/Variant.h>
#include <vnx/query/Aggregate.hxx>
#include <vnx/query/Expression.hxx>


namespace vnx {
namespace query {

class Average : public ::vnx::query::Aggregate {
public:
	
	::std::shared_ptr<const ::vnx::query::Expression> A;
	::vnx::float64_t sum = 0;
	::int64_t count = 0;
	
	typedef ::vnx::query::Aggregate Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	virtual void update(const ::vnx::Object& object);
	virtual ::vnx::Variant get_result() const;
	virtual ::std::string as_string() const;
	
	static std::shared_ptr<Average> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const Average& _value);
	friend std::istream& operator>>(std::istream& _in, Average& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace query

#endif // INCLUDE_vnx_query_Average_HXX_
