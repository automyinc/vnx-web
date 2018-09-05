
#include <vnx/web/FileSystem.h>
#include <vnx/web/BinaryData.hxx>
#include <vnx/Directory.h>


namespace vnx {
namespace web {

FileSystem::FileSystem(const std::string& _vnx_name)
	:	FileSystemBase(_vnx_name)
{
}

void FileSystem::main() {
	
	mime_type_map[".html"] = "text/html";
	mime_type_map[".css"] = "text/css";
	mime_type_map[".js"] = "application/javascript";
	mime_type_map[".json"] = "application/json";
	mime_type_map[".png"] = "image/png";
	mime_type_map[".jpg"] = "image/jpeg";
	mime_type_map[".jpeg"] = "image/jpeg";
	mime_type_map[".txt"] = "text/plain";
	mime_type_map[".sh"] = "text/plain";
	mime_type_map[".cfg"] = "text/plain";
	mime_type_map[".md"] = "text/plain";
	mime_type_map[".h"] = "text/plain";
	mime_type_map[".hpp"] = "text/plain";
	mime_type_map[".hxx"] = "text/plain";
	mime_type_map[".c"] = "text/plain";
	mime_type_map[".cpp"] = "text/plain";
	mime_type_map[".cxx"] = "text/plain";
	mime_type_map[".md5"] = "text/plain";
	mime_type_map[".map"] = "text/plain";
	mime_type_map[".tex"] = "text/x-tex";
	mime_type_map[".pdf"] = "application/pdf";
	
	{
		auto provider = Provider::create();
		provider->id = Hash128::rand();
		provider->path = domain_path;
		provider->input = input;
		this->provider = provider;
	}
	
	cache = std::make_shared<HistoryCache>(max_history_size);
	
	subscribe(input, max_input_queue_ms);
	
	set_timer_millis(update_interval_ms, std::bind(&FileSystem::update, this));
	set_timer_millis(1000, std::bind(&FileSystem::print_stats, this));
	
	Super::main();
}

void FileSystem::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	std::shared_ptr<Response> response = Response::create();
	response->is_for_request(request);
	try {
		switch(request->type) {
			case request_type_e::READ:
				response->result = read_file(request->path);
				response->is_dynamic = false;
				response->time_to_live_ms = time_to_live_ms;
				break;
			case request_type_e::WRITE: {
				auto content = std::dynamic_pointer_cast<const BinaryData>(request->parameter);
				if(content) {
					write_file(request->path, content->data);
				} else {
					response->result = ErrorCode::create(ErrorCode::BAD_REQUEST);
				}
				break;
			}
			default:
				response->result = ErrorCode::create(ErrorCode::BAD_REQUEST);
		}
	} catch(const std::exception& ex) {
		response->result = ErrorCode::create_with_message(ErrorCode::INTERNAL_ERROR, ex.what());
		log(WARN).out << ex.what();
	}
	publish(response, request->get_return_channel());
	request_counter++;
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

std::shared_ptr<const vnx::Value> FileSystem::read_file(const Path& path) {
	
	vnx::File file_path(source_path + path.to_string());
	if(!file_path.exists()) {
		return vnx::clone(ErrorCode::create(ErrorCode::NOT_FOUND));
	}
	
	const int64_t last_write_time_ms = file_path.last_write_time() * 1000;
	{
		std::shared_ptr<const Content> content = cache->query(path);
		if(content) {
			if(last_write_time_ms == content->time_stamp_ms) {
				return content;
			}
		}
	}
	
	std::shared_ptr<Content> result;
	
	if(file_path.is_directory()) {
		
		std::shared_ptr<Directory> directory = Directory::create();
		directory->path = path;
		
		vnx::Directory dir(file_path.get_path());
		dir.open();
		for(const auto& file : dir.all_files()) {
			const std::string name = file->get_name();
			if(is_valid_file(name) && (file->is_directory() || file->is_regular())) {
				FileInfo info;
				info.name = name;
				info.is_directory = file->is_directory();
				info.mime_type = get_mime_type(file->get_extension());
				if(file->is_regular()) {
					info.num_bytes = file->file_size();
				}
				directory->files.push_back(info);
			}
		}
		result = directory;
		
	} else if(file_path.is_regular()) {
		
		const std::string file_name = file_path.get_name();
		if(!is_valid_file(file_name)) {
			return vnx::clone(ErrorCode::create(ErrorCode::NOT_FOUND));
		}
		
		const int64_t file_size = file_path.file_size();
		if(file_size < 0) {
			throw std::runtime_error("Invalid file: " + path.to_string());
		}
		if(max_file_size && file_size > max_file_size) {
			throw std::runtime_error("File too big: " + path.to_string());
		}
		
		std::shared_ptr<File> file = File::create();
		file->name = file_name;
		
		::FILE* p_file = ::fopen(file_path.get_path().c_str(), "rb");
		if(!p_file) {
			throw std::runtime_error("fopen() failed for: " + path.to_string());
		}
		
		file->data.reserve(size_t(file_size));
		const size_t num_read = ::fread(file->data.data(), 1, size_t(file_size), p_file);
		file->data.set_size(num_read);
		
		::fclose(p_file);
		
		result = file;
		num_read_bytes += num_read;
	} else {
		return vnx::clone(ErrorCode::create(ErrorCode::NOT_FOUND));
	}
	
	result->mime_type = get_mime_type(file_path.get_extension());
	result->time_stamp_ms = last_write_time_ms;
	cache->insert(path, result);
	return result;
}

void FileSystem::write_file(const Path& path, const vnx::Memory& data) {
	
	const std::string file_name = source_path + path.to_string();
	const std::string tmp_file_name = file_name + ".tmp";
	
	::FILE* p_file = ::fopen(tmp_file_name.c_str(), "wb");
	if(!p_file) {
		throw std::runtime_error("fopen() failed for: " + path.to_string());
	}
	const Memory::chunk_t* chunk = data.front();
	while(chunk) {
		const size_t num_write = ::fwrite(chunk->data(), 1, chunk->size(), p_file);
		if(num_write != chunk->size()) {
			::fclose(p_file);
			::remove(tmp_file_name.c_str());
			throw std::runtime_error("fwrite() failed for: " + path.to_string());
		}
		num_write_bytes += num_write;
		chunk = chunk->next();
	}
	if(::fclose(p_file)) {
		throw std::runtime_error("fclose() failed for: " + path.to_string());
	}
	
	if(::rename(tmp_file_name.c_str(), file_name.c_str())) {
		throw std::runtime_error("rename() failed for: " + path.to_string());
	}
	
	cache->remove(path);
}

bool FileSystem::is_valid_file(const std::string& file_name) {
	return !file_name.empty() && file_name.front() != '.';
}

std::string FileSystem::get_mime_type(const std::string& extension) const {
	auto iter = mime_type_map.find(extension);
	if(iter != mime_type_map.end()) {
		return iter->second;
	}
	return "application/octet-stream";
}

} // web
} // vnx
