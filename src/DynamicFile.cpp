
#include <vnx/web/DynamicFile.hxx>
#include <vnx/Input.h>
#include <vnx/Time.h>

#include <sstream>


namespace vnx {
namespace web {

void DynamicFile::parse() {
	
	code.clear();
	keys.clear();
	chunks.clear();
	static_context.clear();
	static_context["mime_type"] = mime_type;
	
	vnx::BufferInputStream stream(&data);
	vnx::InputBuffer in(&stream);
	std::string current;
	try {
		int state = 0;
		int stack = 0;
		std::string key;
		std::string value;
		bool is_string = false;
		bool is_escape = false;
		bool do_run = true;
		while(do_run) {
			const char c = *in.read(1);
			switch(state) {
				case 0:
					if(c == '{') {
						state = 1;
					} else {
						current += c;
					}
					break;
				case 1:
					if(c == '{') {
						if(!current.empty()) {
							code.push_back(CODE_TEXT);
							code.push_back(int(chunks.size()));
							chunks.push_back(current);
							current.clear();
						}
						state = 2;
					} else if(c == '!') {
						current += '{';
					} else {
						current += '{';
						current += c;
						state = 0;
					}
					break;
				case 2:
					if(c == '}') {
						state = 4;
					} else if(c == '=') {
						state = 3;
					} else if(c != ' ' && c != '\t' && c != '\r' && c != '\n') {
						key += c;
					}
					break;
				case 3:
					if(is_string) {
						if(is_escape) {
							if(c != '"') {
								value += '\\';
							}
							value += c;
							is_escape = false;
						} else if(c == '\\') {
							is_escape = true;
						} else if(c == '"') {
							is_string = false;
							if(stack == 0) {
								state = 5;
							} else {
								value += c;
							}
						} else {
							value += c;
						}
					} else {
						if(c == '"') {
							if(stack > 0) {
								value += c;
							}
							is_string = true;
						} else if(c == '}') {
							if(stack == 0) {
								state = 4;
							} else {
								stack--;
								value += c;
							}
						} else if(c == '{') {
							stack++;
							value += c;
						} else if(c != ' ' && c != '\t' && c != '\r' && c != '\n') {
							value += c;
						}
					}
					break;
				case 4:
					if(c == '}') {
						if(value.empty()) {
							code.push_back(CODE_INSERT);
							code.push_back(int(keys.size()));
							keys.push_back(key);
						} else {
							static_context[key] = value;
						}
						key.clear();
						value.clear();
						state = 0;
					} else {
						do_run = false;
					}
					break;
				case 5:
					if(c == '}') {
						state = 4;
					}
					break;
				default:
					do_run = false;
			}
		}
	} catch(...) {
		// EOF
	}
	
	if(!current.empty()) {
		code.push_back(CODE_TEXT);
		code.push_back(int(chunks.size()));
		chunks.push_back(current);
	}
	
	mime_type = static_context["mime_type"].to<std::string>();
}

std::shared_ptr<const vnx::web::File> DynamicFile::execute(const ::vnx::Object& context) const {
	return execute_with_options(context, false);
}

std::shared_ptr<const vnx::web::File> DynamicFile::execute_with_options(const ::vnx::Object& context, const ::vnx::bool_t& allow_nesting) const {
	
	auto file = File::create();
	file->name = name;
	file->mime_type = mime_type;
	file->time_stamp_ms = vnx::get_time_millis();
	
	std::ostringstream result;
	for(size_t i = 0; i < code.size(); ++i) {
		switch(code[i]) {
			case CODE_TEXT:
				if(i + 1 < code.size() && code[i + 1] < chunks.size()) {
					result << chunks[code[i + 1]];
				}
				i++;
				break;
			case CODE_INSERT:
				if(i + 1 < code.size() && code[i + 1] < keys.size()) {
					const std::string& key = keys[code[i + 1]];
					auto iter = context.field.find(key);
					if(iter != context.field.end()) {
						result << iter->second.to<std::string>();
					} else {
						iter = static_context.field.find(key);
						if(iter != static_context.field.end()) {
							result << iter->second.to<std::string>();
						} else {
							if(allow_nesting) {
								result << "{{" << key << "}}";
							}
						}
					}
				}
				i++;
				break;
		}
	}
	file->data = result.str();
	return file;
}


} // web
} // vnx
