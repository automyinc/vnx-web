
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_FileSystemBase_HXX_
#define INCLUDE_vnx_web_FileSystemBase_HXX_

#include <vnx/web/package.hxx>
#include <vnx/Module.h>
#include <vnx/TopicPtr.h>
#include <vnx/web/HttpRequest.hxx>
#include <vnx/web/Path.h>
#include <vnx/web/Request.hxx>


namespace vnx {
namespace web {

class FileSystemBase : public ::vnx::Module {
public:
	
	::vnx::TopicPtr domain;
	::vnx::TopicPtr channel;
	::std::string source_path = ".";
	::vnx::web::Path provider_path = "/";
	::int32_t update_interval_ms = 500;
	::int32_t scan_interval_ms = 30000;
	::int64_t time_to_live_ms = 60000;
	::int64_t max_file_size = 0;
	
	typedef ::vnx::Module Super;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	FileSystemBase(const std::string& _vnx_name);
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	friend std::ostream& operator<<(std::ostream& _out, const FileSystemBase& _value);
	friend std::istream& operator>>(std::istream& _in, FileSystemBase& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
protected:
	virtual void handle(std::shared_ptr<const ::vnx::web::Request> _value, std::shared_ptr<const ::vnx::Sample> _sample) { handle(_value); }
	virtual void handle(std::shared_ptr<const ::vnx::web::Request> _value) {}
	virtual void scan() = 0;
	
	void handle_switch(std::shared_ptr<const ::vnx::Sample> _sample);
	bool call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type);
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_FileSystemBase_HXX_
