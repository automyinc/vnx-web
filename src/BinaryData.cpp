
#include <vnx/web/BinaryData.hxx>
#include <vnx/Output.h>

#include <cstring>


namespace vnx {
namespace web {

void BinaryData::write_value(const vnx::Value& value) {
	data.clear();
	MemoryOutputStream stream(&data);
	TypeOutput out(&stream);
	vnx::write(out, uint16_t(CODE_NONE));		// optional file header
	vnx::write(out, uint16_t(CODE_MAGIC));		// optional file header
	vnx::write(out, value);
	out.flush();
}

void BinaryData::write_string(const std::string& value) {
	data.clear();
	::memcpy(data.add_chunk(value.size()), value.data(), value.size());
}

uint64_t BinaryData::get_size() const {
	return data.get_size();
}

std::string BinaryData::as_string() const {
	return data.as_string();
}


} // web
} // vnx
