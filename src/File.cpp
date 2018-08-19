
#include <vnx/web/File.hxx>


namespace vnx {
namespace web {

::int64_t File::get_num_bytes() const {
	return data.size();
}


} // web
} // vnx
