
#include <vnx/web/File.hxx>


namespace vnx {
namespace web {

::int64_t File::get_num_bytes() const {
	return data.size();
}

::std::string File::get_name_extension() const {
	const size_t pos = name.find_last_of('.');
	if(pos != std::string::npos) {
		return name.substr(pos);
	}
	return "";
}


} // web
} // vnx
