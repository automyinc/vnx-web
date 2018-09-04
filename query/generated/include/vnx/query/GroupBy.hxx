
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_query_GroupBy_HXX_
#define INCLUDE_vnx_query_GroupBy_HXX_

#include <vnx/query/package.hxx>
#include <vnx/Value.h>


namespace vnx {
namespace query {

class GroupBy : public ::vnx::Value {
public:
	
	::std::string field;
	
	typedef ::vnx::Value Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	virtual ::std::string as_string() const;
	
	static std::shared_ptr<GroupBy> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	friend std::ostream& operator<<(std::ostream& _out, const GroupBy& _value);
	friend std::istream& operator>>(std::istream& _in, GroupBy& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace query

#endif // INCLUDE_vnx_query_GroupBy_HXX_
