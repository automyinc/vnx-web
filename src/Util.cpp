
#include <vnx/web/Util.h>

#include <ctime>


namespace vnx {
namespace web {

std::string to_date_string(int64_t time_stamp_s) {
	const ::time_t time = time_stamp_s;
	char buf[256];
	// <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
	const size_t size = ::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", ::localtime(&time));
	return std::string(buf, size);
}


} // web
} // vnx
