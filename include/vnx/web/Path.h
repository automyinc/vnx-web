
#ifndef INCLUDE_VNX_WEB_PATH_H
#define INCLUDE_VNX_WEB_PATH_H

#include <vnx/Type.h>


namespace vnx {
namespace web {

class Path : public std::vector<std::string> {
public:
	Path() = default;
	
	Path(const std::string& path) {
		assign(path);
	}
	
	Path(const Path&) = default;
	
	Path& operator=(const std::string& path) {
		assign(path);
	}
	
	bool operator==(const Path& other) const;
	
	bool operator!=(const Path& other) const;
	
	bool operator<(const Path& other) const;
	
	bool operator>(const Path& other) const;
	
	void assign(const std::string& path);
	
	std::string to_string() const;
	
	friend std::ostream& operator<<(std::ostream& out, const Path& path);
	
	friend std::istream& operator>>(std::istream& in, Path& path);
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_PATH_H
