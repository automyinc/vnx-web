
#include <vnx/web/Path.h>

#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>


namespace vnx {
namespace web {

void Path::assign(const std::string& path) {
	// TODO
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

std::string Path::to_string() const {
	std::string path;
	for(const std::string& node : *this) {
		if(!path.empty()) {
			path.push_back('/');
		}
		path += node;
	}
	return path;
}

std::ostream& operator<<(std::ostream& out, const Path& path) {
	return out << path.to_string();
}

std::istream& operator>>(std::istream& in, Path& path) {
	std::string tmp;
	in >> tmp;
	path.assign(tmp);
	return in;
}

void read(TypeInput& in, Path& value, const TypeCode* type_code, const uint16_t* code) {
	vnx::read(in, (std::vector<std::string>&)value, type_code, code);
}

void write(TypeOutput& out, const Path& value, const TypeCode* type_code, const uint16_t* code) {
	vnx::write(out, (const std::vector<std::string>&)value, type_code, code);
}

void read(std::istream& in, Path& value) {
	vnx::read(in, (std::vector<std::string>&)value);
}

void write(std::ostream& out, const Path& value) {
	vnx::write(out, (const std::vector<std::string>&)value);
}

void accept(Visitor& visitor, const Path& value) {
	vnx::accept(visitor, (const std::vector<std::string>&)value);
}


} // web
} // vnx
