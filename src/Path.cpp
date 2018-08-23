
#include <vnx/web/package.hxx>
#include <vnx/web/Path.h>

#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Util.h>
#include <vnx/CRC64.h>


namespace vnx {
namespace web {

void Path::assign(const std::string& path) {
	std::vector<std::string>::operator=(vnx::string_split(path, '/'));
}

bool Path::operator==(const Path& other) const {
	return to_string() == other.to_string();
}

bool Path::operator!=(const Path& other) const {
	return !(*this == other);
}

bool Path::operator<(const Path& other) const {
	return to_string() < other.to_string();
}

bool Path::operator>(const Path& other) const {
	return other < *this;
}

Path& Path::operator+=(const Path& other) {
	if(!empty() && back().empty()) {
		pop_back();
	}
	insert(end(), !other.empty() && other.front().empty() ? ++other.begin() : other.begin(), other.end());
	return *this;
}

Path Path::operator+(const Path& other) const {
	Path result = *this;
	result += other;
	return result;
}

Hash64 Path::get_hash() const {
	CRC64 func;
	for(const std::string& node : (*this)) {
		func.update(node);
	}
	return Hash64(func.get());
}

std::string Path::to_string() const {
	std::string path;
	for(size_t i = 0; i < size(); ++i) {
		if(i) {
			path.push_back('/');
		}
		path += (*this)[i];
	}
	return path;
}

bool Path::has_parent_path() const {
	return !get_parent_path().empty();
}

Path Path::get_parent_path() const {
	Path parent = *this;
	if(!empty()) {
		if(back().empty()) {
			parent.pop_back();
			if(!parent.empty()) {
				if(!parent.back().empty()) {
					parent.back() = "";
				} else {
					parent.pop_back();
				}
			}
		} else {
			parent.pop_back();
		}
	}
	return parent;
}

Path Path::get_base_path(size_t N) const {
	if(N > size()) {
		N = size();
	}
	Path result;
	for(size_t i = 0; i < N; ++i) {
		result.push_back((*this)[i]);
	}
	return result;
}

Path Path::get_sub_path(size_t B) const {
	Path result;
	for(size_t i = B; i < size(); ++i) {
		result.push_back((*this)[i]);
	}
	return result;
}

std::ostream& operator<<(std::ostream& out, const Path& path) {
	vnx::write(out, path.to_string());
	return out;
}

std::istream& operator>>(std::istream& in, Path& path) {
	vnx::read(in, path);
	return in;
}


} // web


void read(TypeInput& in, vnx::web::Path& value, const TypeCode* type_code, const uint16_t* code) {
	std::string path;
	vnx::read(in, path, type_code, code);
	value = path;
}

void write(TypeOutput& out, const vnx::web::Path& value, const TypeCode* type_code, const uint16_t* code) {
	vnx::write(out, value.to_string(), type_code, code);
}

void read(std::istream& in, vnx::web::Path& value) {
	std::string path;
	vnx::read(in, path);
	value = path;
}

void write(std::ostream& out, const vnx::web::Path& value) {
	vnx::write(out, value.to_string());
}

void accept(Visitor& visitor, const vnx::web::Path& value) {
	vnx::accept(visitor, value.to_string());
}

} // vnx
