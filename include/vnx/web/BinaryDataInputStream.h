
#ifndef INCLUDE_VNX_WEB_BINARYDATAINPUTSTREAM_H
#define INCLUDE_VNX_WEB_BINARYDATAINPUTSTREAM_H

#include <vnx/web/BinaryData.hxx>
#include <vnx/Input.h>


namespace vnx {
namespace web {

class BinaryDataInputStream : public vnx::InputStream {
public:
	BinaryDataInputStream(const BinaryData* data_) : data(data_) {}
	
	size_t read(void* buf, size_t len) override {
		size_t res = 0;
		while(index < data->chunks.size()) {
			const vnx::Buffer& chunk = data->chunks[index];
			if(chunk.size() > offset) {
				const size_t left = chunk.size() - offset;
				const size_t num_bytes = left < len ? left : len;
				::memcpy(buf, chunk.data(offset), num_bytes);
				if(num_bytes == left) {
					index++;
					offset = 0;
				} else {
					offset += num_bytes;
				}
				res = num_bytes;
				break;
			} else {
				index++;
				offset = 0;
			}
		}
		return res;
	}
	
private:
	const BinaryData* data = 0;
	size_t index = 0;
	size_t offset = 0;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_BINARYDATAINPUTSTREAM_H
