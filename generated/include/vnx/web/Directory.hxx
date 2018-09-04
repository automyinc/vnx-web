
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_Directory_HXX_
#define INCLUDE_vnx_web_Directory_HXX_

#include <vnx/web/package.hxx>
#include <vnx/web/Content.hxx>
#include <vnx/web/FileInfo.hxx>
#include <vnx/web/Path.h>


namespace vnx {
namespace web {

class Directory : public ::vnx::web::Content {
public:
	
	::vnx::web::Path path;
	::std::vector<::vnx::web::FileInfo> files;
	
	typedef ::vnx::web::Content Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	static std::shared_ptr<Directory> create();
	std::shared_ptr<vnx::Value> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	vnx::Object to_object() const;
	void from_object(const vnx::Object& object);
	
	friend std::ostream& operator<<(std::ostream& _out, const Directory& _value);
	friend std::istream& operator>>(std::istream& _in, Directory& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_Directory_HXX_
