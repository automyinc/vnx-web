
#include <vnx/web/BinaryData.hxx>
#include <vnx/web/BinaryDataOutputStream.h>
#include <vnx/Output.h>

#include <cstring>


namespace vnx {
namespace web {

void BinaryData::append(const BinaryData& data) {
	chunks.insert(chunks.end(), data.chunks.begin(), data.chunks.end());
}

void BinaryData::write_value(const vnx::Value& data) {
	chunks.clear();
	BinaryDataOutputStream stream(this);
	TypeOutput out(&stream);
	vnx::write(out, uint16_t(CODE_NONE));		// optional file header
	vnx::write(out, uint16_t(CODE_MAGIC));		// optional file header
	vnx::write(out, data);
	out.flush();
}

void BinaryData::write_string(const std::string& data) {
	chunks.clear();
	chunks.emplace_back(data.size());
	::memcpy(chunks.back().data(), data.data(), data.size());
}

uint64_t BinaryData::get_size() const {
	uint64_t size = 0;
	for(const auto& chunk : chunks) {
		size += chunk.size();
	}
	return size;
}

vnx::Buffer BinaryData::as_buffer() const {
	vnx::Buffer result(get_size());
	size_t offset = 0;
	for(const auto& chunk : chunks) {
		::memcpy((char*)result.data() + offset, chunk.data(), chunk.size());
		offset += chunk.size();
	}
	return result;
}

std::string BinaryData::as_string() const {
	std::string result;
	result.resize(get_size());
	size_t offset = 0;
	for(const auto& chunk : chunks) {
		::memcpy(&result[offset], chunk.data(), chunk.size());
		offset += chunk.size();
	}
	return result;
}


} // web
} // vnx
