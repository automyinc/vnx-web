
#ifndef INCLUDE_VNX_WEB_PATH_H
#define INCLUDE_VNX_WEB_PATH_H

#include <vnx/Type.h>
#include <vnx/Hash64.h>


namespace vnx {
namespace web {

class Path : public std::vector<std::string> {
public:
	Path() = default;
	
	Path(const char path[]) {
		assign(std::string(path));
	}
	
	Path(const std::string& path) {
		assign(path);
	}
	
	Path(const Path&) = default;
	
	Path& operator=(const char path[]) {
		assign(std::string(path));
	}
	
	Path& operator=(const std::string& path) {
		assign(path);
	}
	
	bool operator==(const Path& other) const;
	
	bool operator!=(const Path& other) const;
	
	bool operator<(const Path& other) const;
	
	bool operator>(const Path& other) const;
	
	Path& operator+=(const Path& other);
	
	Path operator+(const Path& other) const;
	
	void assign(const std::string& path);
	
	Hash64 get_hash() const;
	
	std::string to_string() const;
	
	bool has_parent_path() const;
	
	bool is_relative() const;
	
	Path get_parent_path() const;
	
	Path get_base_path(size_t N) const;
	
	Path get_sub_path(size_t N) const;
	
	Path get_relative_path(const Path& base_path) const;
	
	Path as_relative_path() const;
	
	friend std::ostream& operator<<(std::ostream& out, const Path& path);
	
	friend std::istream& operator>>(std::istream& in, Path& path);
	
};


} // web
} // vnx


namespace std {
	template<> struct hash<vnx::web::Path> {
		size_t operator()(const vnx::web::Path& x) const {
			return x.get_hash();
		}
	};
} // std

#endif // INCLUDE_VNX_WEB_PATH_H
