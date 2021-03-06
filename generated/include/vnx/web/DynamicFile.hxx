
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_DynamicFile_HXX_
#define INCLUDE_vnx_web_DynamicFile_HXX_

#include <vnx/web/package.hxx>
#include <vnx/Object.h>
#include <vnx/web/File.hxx>


namespace vnx {
namespace web {

class DynamicFile : public ::vnx::web::File {
public:
	static const ::int32_t CODE_TEXT = 1;
	static const ::int32_t CODE_INSERT = 2;
	
	::std::vector<::int32_t> code;
	::std::vector<::std::string> keys;
	::std::vector<::std::string> chunks;
	::vnx::Object static_context;
	
	typedef ::vnx::web::File Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	virtual void parse();
	virtual ::std::shared_ptr<const ::vnx::web::File> execute(const ::vnx::Object& context) const;
	virtual ::std::shared_ptr<const ::vnx::web::File> execute_with_options(const ::vnx::Object& context, const ::vnx::bool_t& allow_nesting) const;
	
	static std::shared_ptr<DynamicFile> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const DynamicFile& _value);
	friend std::istream& operator>>(std::istream& _in, DynamicFile& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_DynamicFile_HXX_
