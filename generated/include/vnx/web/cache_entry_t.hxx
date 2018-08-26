
// AUTO GENERATED by vnxcppcodegen

#ifndef INCLUDE_vnx_web_cache_entry_t_HXX_
#define INCLUDE_vnx_web_cache_entry_t_HXX_

#include <vnx/Type.h>
#include <vnx/web/package.hxx>
#include <vnx/web/Content.hxx>
#include <vnx/web/Path.h>


namespace vnx {
namespace web {

struct cache_entry_t {
	
	
	::vnx::web::Path path;
	::std::shared_ptr<const ::vnx::web::Content> content;
	::int64_t time_stamp_ms = 0;
	::int64_t time_to_live_ms = 0;
	::int64_t last_request_ms = 0;
	::int64_t num_pending = 0;
	::int64_t num_hits = 0;
	
	static const vnx::Hash64 VNX_TYPE_HASH;
	static const vnx::Hash64 VNX_CODE_HASH;
	
	vnx::Hash64 get_type_hash() const;
	const char* get_type_name() const;
	
	static std::shared_ptr<cache_entry_t> create();
	std::shared_ptr<cache_entry_t> clone() const;
	
	void read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code);
	void write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const;
	
	void read(std::istream& _in);
	void write(std::ostream& _out) const;
	
	void accept(vnx::Visitor& _visitor) const;
	
	friend std::ostream& operator<<(std::ostream& _out, const cache_entry_t& _value);
	friend std::istream& operator>>(std::istream& _in, cache_entry_t& _value);
	
	static const vnx::TypeCode* get_type_code();
	static std::shared_ptr<vnx::TypeCode> create_type_code();
	
};


} // namespace vnx
} // namespace web

#endif // INCLUDE_vnx_web_cache_entry_t_HXX_
