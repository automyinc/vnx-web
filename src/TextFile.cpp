
#include <vnx/web/TextFile.hxx>


namespace vnx {
namespace web {

::int64_t TextFile::get_num_bytes() const {
	return data.size();
}


} // web
} // vnx
