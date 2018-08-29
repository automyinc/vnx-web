
#ifndef INCLUDE_VNX_WEB_FILESYSTEM_H
#define INCLUDE_VNX_WEB_FILESYSTEM_H

#include <vnx/web/FileSystemBase.hxx>
#include <vnx/web/Response.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/web/File.hxx>
#include <vnx/web/Directory.hxx>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/web/HistoryCache.h>
#include <vnx/Memory.h>

#include <boost/filesystem.hpp>

#include <queue>
#include <unordered_map>

struct magic_set;


namespace vnx {
namespace web {

class FileSystem : public FileSystemBase {
public:
	FileSystem(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::Request> request) override;
	
private:
	void update();
	
	void print_stats();
	
	std::shared_ptr<const vnx::Value> read_file(const Path& path);
	
	void write_file(const Path& path, const vnx::Memory& data);
	
	bool is_valid_file(const std::string& file_name);
	
	std::string get_mime_type(const std::string& path, const std::string& extension);
	
	std::string get_mime_type_based_on_extension(const std::string& extension);
	
private:
	std::shared_ptr<HistoryCache> cache;
	std::shared_ptr<const Provider> provider;
	
	::magic_set* magic = 0;
	size_t request_counter = 0;
	size_t num_read_bytes = 0;
	size_t num_write_bytes = 0;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_FILESYSTEM_H
