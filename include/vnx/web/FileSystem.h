
#ifndef INCLUDE_VNX_WEB_FILESYSTEM_H
#define INCLUDE_VNX_WEB_FILESYSTEM_H

#include <vnx/web/FileSystemBase.hxx>
#include <vnx/web/Response.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/web/File.hxx>
#include <vnx/web/Directory.hxx>
#include <vnx/web/ErrorCode.hxx>

#include <boost/filesystem.hpp>
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
	
	void scan() override;
	
private:
	void update();
	
	std::shared_ptr<Content> read_file(const Path& path);
	
	void scan_tree(const Path& current, const boost::filesystem::path& file_path);
	
	bool is_valid_file(const std::string& file_name);
	
	std::string get_mime_type(const std::string& path);
	
private:
	struct file_entry_t {
		boost::filesystem::path path;
		std::string mime_type;
		int64_t time_stamp_ms = 0;
	};
	
	std::shared_ptr<const Provider> provider;
	std::unordered_map<Path, file_entry_t> file_map;
	
	::magic_set* magic = 0;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_FILESYSTEM_H
