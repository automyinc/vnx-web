
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
	magic = ::magic_open(MAGIC_MIME_TYPE);
	if(magic) {
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
		return;
	}
	
	{
		auto provider = Provider::create();
		provider->id = Hash128::rand();
		provider->path = domain_path;
		provider->input = input;
		this->provider = provider;
	}
	
	cache = std::make_shared<HistoryCache>(max_history_size);
	
	scan();
	
	subscribe(input, max_input_queue_ms);
	
	set_timer_millis(update_interval_ms, std::bind(&FileSystem::update, this));
	set_timer_millis(scan_interval_ms, std::bind(&FileSystem::scan, this));
	set_timer_millis(1000, std::bind(&FileSystem::print_stats, this));
	
	Super::main();
	
	::magic_close(magic);
}

void FileSystem::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	std::shared_ptr<Response> response = Response::create();
	response->is_for_request(request);
	response->is_dynamic = false;
	response->time_to_live_ms = time_to_live_ms;
	try {
		switch(request->type) {
			case request_type_e::READ:
				response->content = read_file(request->path);
				break;
			case request_type_e::WRITE: {
				auto content = std::dynamic_pointer_cast<const BinaryData>(request->parameter);
				if(content) {
					write_file(request->path, content);
				} else {
					response->content = ErrorCode::create(ErrorCode::BAD_REQUEST);
				}
				break;
			}
			default:
				response->content = ErrorCode::create(ErrorCode::BAD_REQUEST);
		}
	} catch(const std::exception& ex) {
		response->content = ErrorCode::create_with_message(ErrorCode::INTERNAL_ERROR, ex.what());
		log(WARN).out << ex.what();
	}
	publish(response, request->get_return_channel());
	request_counter++;
}

void FileSystem::scan() {
	const filesystem::path root = source_path;
	if(filesystem::exists(root)) {
		if(filesystem::is_directory(root)) {
			try {
				scan_tree("/", root);
			} catch(const std::exception& ex) {
				log(WARN).out << ex.what();
			}
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

void FileSystem::print_stats() {
	log(INFO).out << "requests=" << request_counter << "/s, read="
			<< (float(num_read_bytes) / 1024 / 1024) << " MB/s, write="
			<< (float(num_write_bytes) / 1024 / 1024) << " MB/s";
	request_counter = 0;
	num_read_bytes = 0;
	num_write_bytes = 0;
}

std::shared_ptr<const Content> FileSystem::read_file(const Path& path) {
	{
		std::shared_ptr<const Content> content = cache->query(path);
		if(content) {
			return content;
		}
	}
	auto entry = file_map.find(path);
	if(entry != file_map.end()) {
		const filesystem::path file_path = entry->second.path;
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
					if(filesystem::is_regular(file_path)) {
						info.num_bytes = filesystem::file_size(file);
					}
					directory->files.push_back(info);
				}
			}
			result = directory;
		} else if(filesystem::is_regular(file_path)) {
			const std::string file_path_str = file_path.generic_string();
			const size_t file_size = filesystem::file_size(file_path);
			if(max_file_size && file_size > max_file_size) {
				throw std::runtime_error("file too big: " + path.to_string() + " (" + std::to_string(file_size) + ")");
			}
			std::shared_ptr<File> file = File::create();
			file->data.reserve(file_size);
			::FILE* p_file = ::fopen(file_path_str.c_str(), "r");
			if(!p_file) {
				throw std::runtime_error("fopen() failed for: " + path.to_string());
			}
			const size_t num_read = ::fread(file->data.data(), 1, file_size, p_file);
			file->data.set_size(num_read);
			::fclose(p_file);
			result = file;
			num_read_bytes += num_read;
		}
		result->path = path;
		result->mime_type = entry->second.mime_type;
		result->time_stamp_ms = entry->second.time_stamp_ms;
		cache->insert(result);
		return result;
	} else {
		return vnx::clone(ErrorCode::create(ErrorCode::NOT_FOUND));
	}
}

void FileSystem::write_file(const Path& path, std::shared_ptr<const BinaryData> content) {
	
	const filesystem::path file_path = source_path + path.to_string();
	if(file_path.has_parent_path()) {
		const filesystem::path parent = file_path.parent_path();
		if(!filesystem::exists(parent)) {
			filesystem::create_directories(parent);
		}
	}
	
	const std::string file_path_str = file_path.generic_string();
	::FILE* p_file = ::fopen(file_path_str.c_str(), "wb");
	if(!p_file) {
		throw std::runtime_error("fopen() failed for: " + path.to_string());
	}
	for(const vnx::Buffer& chunk : content->chunks) {
		const size_t num_write = ::fwrite(chunk.data(), 1, chunk.size(), p_file);
		if(num_write != chunk.size()) {
			::fclose(p_file);
			throw std::runtime_error("fwrite() failed for: " + path.to_string());
		}
		num_write_bytes += num_write;
	}
	::fclose(p_file);
	
	file_entry_t& entry = file_map[path];
	entry.path = file_path;
	entry.mime_type = get_mime_type(content);
	entry.time_stamp_ms = int64_t(filesystem::last_write_time(file_path)) * 1000;
	cache->remove(path);
}

void FileSystem::scan_tree(const Path& current, const filesystem::path& file_path) {
	Path path = current;
	file_entry_t* entry = 0;
	if(filesystem::is_directory(file_path)) {
		for(auto it = filesystem::directory_iterator(file_path); it != filesystem::directory_iterator(); ++it) {
			const std::string file_name = it->path().filename().generic_string();
			if(is_valid_file(file_name)) {
				scan_tree(current + file_name, it->path());
			}
		}
		path += "/";
		entry = &file_map[path];
	} else if(filesystem::is_regular(file_path)) {
		entry = &file_map[path];
	}
	if(entry) {
		const int64_t time = int64_t(filesystem::last_write_time(file_path)) * 1000;
		if(time > entry->time_stamp_ms) {
			if(entry->time_stamp_ms) {
				cache->remove(path);	// clear file from cache
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
	return "application/octet-stream";
}

std::string FileSystem::get_mime_type(std::shared_ptr<const BinaryData> content) {
	if(content && !content->chunks.empty()) {
		char const * mime = ::magic_buffer(magic, content->chunks.front().data(), content->chunks.front().size());
		if(mime) {
			return std::string(mime);
		}
	}
	return "application/octet-stream";
}


} // web
} // vnx
