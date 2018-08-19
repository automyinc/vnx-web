
#include <vnx/web/FileSystem.h>

#include <magic.h>

using namespace boost;


namespace vnx {
namespace web {

FileSystem::FileSystem(const std::string& _vnx_name)
	:	FileSystemBase(_vnx_name)
{
}

void FileSystem::main() {
	if(magic = ::magic_open(MAGIC_MIME_TYPE)) {
		if(::magic_load(magic, nullptr) == 0) {
			if(::magic_compile(magic, nullptr) == 0) {
				// success
			} else {
				log(ERROR).out << "magic_compile() failed!";
			}
		} else {
			log(ERROR).out << "magic_load() failed!";
		}
	} else {
		log(ERROR).out << "magic_open(MAGIC_MIME_TYPE) failed!";
	}
	
	{
		auto provider = Provider::create();
		provider->id = Hash128::rand();
		provider->path = provider_path.get_base_path(provider_path.size() - 1);
		provider->channel = channel;
		this->provider = provider;
	}
	
	scan();
	
	subscribe(channel);
	
	set_timer_millis(update_interval_ms, std::bind(&FileSystem::update, this));
	set_timer_millis(scan_interval_ms, std::bind(&FileSystem::scan, this));
	
	Super::main();
	
	if(magic) {
		::magic_close(magic);
	}
}

void FileSystem::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	std::shared_ptr<Response> response = Response::create();
	response->id = request->id;
	response->is_dynamic = false;
	response->time_to_live_ms = time_to_live_ms;
	try {
		response->content = read_file(request->path);
	} catch(const std::exception& ex) {
		response->content = vnx::clone(ErrorCode::create(ErrorCode::INTERNAL_ERROR));
		log(WARN).out << ex.what();
	}
	publish(response, request->channel);
}

void FileSystem::scan() {
	const filesystem::path root = source_path;
	if(filesystem::exists(root)) {
		if(filesystem::is_directory(root)) {
			scan_tree(provider_path, root);
		} else {
			log(WARN).out << "Not a directory: " << root;
		}
	} else {
		log(WARN).out << "No such file: " << root;
	}
}

void FileSystem::update() {
	publish(provider, domain);
}

std::shared_ptr<Content> FileSystem::read_file(const Path& path) {
	auto entry = file_map.find(path);
	if(entry != file_map.end()) {
		const filesystem::path file_path = entry->second.path;
		const std::string source_path = file_path.generic_string();
		std::shared_ptr<Content> result;
		if(filesystem::is_directory(file_path)) {
			std::shared_ptr<Directory> directory = Directory::create();
			for(auto it = filesystem::directory_iterator(file_path); it != filesystem::directory_iterator(); ++it) {
				const filesystem::path file = it->path();
				const std::string file_name = file.filename().generic_string();
				if(is_valid_file(file_name)) {
					FileInfo info;
					info.name = file_name;
					info.is_directory = filesystem::is_directory(file);
					if(!info.is_directory) {
						info.num_bytes = filesystem::file_size(file);
					}
					directory->files.push_back(info);
				}
			}
			result = directory;
		} else if(filesystem::is_regular(file_path)) {
			const size_t file_size = filesystem::file_size(file_path);
			if(max_file_size && file_size > max_file_size) {
				throw std::runtime_error("file too big: " + path.to_string() + " (" + std::to_string(file_size) + ")");
			}
			std::shared_ptr<File> file = File::create();
			file->data.resize(file_size);
			::FILE* p_file = ::fopen(source_path.c_str(), "r");
			if(!p_file) {
				throw std::runtime_error("fopen() failed for: " + path.to_string());
			}
			const size_t num_read = ::fread(file->data.data(), 1, file->data.size(), p_file);
			if(num_read != file->data.size()) {
				file->data.resize(num_read);
			}
			::fclose(p_file);
			result = file;
		}
		result->path = path;
		result->mime_type = entry->second.mime_type;
		result->time_stamp_ms = entry->second.time_stamp_ms;
		return result;
	} else {
		return vnx::clone(ErrorCode::create(ErrorCode::NOT_FOUND));
	}
}

void FileSystem::scan_tree(const Path& current, const filesystem::path& file_path) {
	file_entry_t* entry = 0;
	if(filesystem::is_directory(file_path)) {
		for(auto it = filesystem::directory_iterator(file_path); it != filesystem::directory_iterator(); ++it) {
			const std::string file_name = it->path().filename().generic_string();
			if(is_valid_file(file_name)) {
				scan_tree(current + file_name, it->path());
			}
		}
		entry = &file_map[current + "/"];
	} else if(filesystem::is_regular(file_path)) {
		entry = &file_map[current];
	}
	if(entry) {
		const int64_t time = int64_t(filesystem::last_write_time(file_path)) * 1000;
		if(time > entry->time_stamp_ms) {
			if(entry->time_stamp_ms) {
				log(INFO).out << "UPDATE " << current << " => " << file_path;
			} else {
				log(INFO).out << "NEW " << current << " => " << file_path;
			}
			entry->path = file_path;
			entry->mime_type = get_mime_type(file_path.generic_string());
			entry->time_stamp_ms = time;
		}
	}
}

bool FileSystem::is_valid_file(const std::string& file_name) {
	return !file_name.empty() && file_name[0] != '.';
}

std::string FileSystem::get_mime_type(const std::string& path) {
	char const * mime = ::magic_file(magic, path.c_str());
	if(mime) {
		return std::string(mime);
	}
	return "unknown";
}


} // web
} // vnx
