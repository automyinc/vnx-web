
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_query_Update_HXX_
#define INCLUDE_vnx_query_Update_HXX_

#include <vnx/query/package.hxx>
#include <vnx/Object.h>
#include <vnx/query/Expression.hxx>
#include <vnx/query/Query.hxx>


namespace vnx {
namespace query {

class Update : public ::vnx::query::Query {
public:
	
	::std::string table;
	::vnx::Object set;
	::std::shared_ptr<const ::vnx::query::Expression> where;
	
	typedef ::vnx::query::Query Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	static std::shared_ptr<Update> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	friend std::ostream& operator<<(std::ostream& _out, const Update& _value);
	friend std::istream& operator>>(std::istream& _in, Update& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace query

#endif // INCLUDE_vnx_query_Update_HXX_
