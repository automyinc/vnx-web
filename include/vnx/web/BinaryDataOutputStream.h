
#ifndef INCLUDE_VNX_WEB_BINARYDATAOUTPUTSTREAM_H
#define INCLUDE_VNX_WEB_BINARYDATAOUTPUTSTREAM_H

#include <vnx/web/BinaryData.hxx>
#include <vnx/Output.h>


namespace vnx {
namespace web {

class BinaryDataOutputStream : public vnx::OutputStream {
public:
	BinaryDataOutputStream(BinaryData* out_) : out(out_) {}
	
	void write(const void* buf, size_t len) override {
		out->chunks.emplace_back(len);
		::memcpy(out->chunks.back().data(), buf, len);
		out->chunks.back().set_size(len);
		written += len;
	}
	
	size_t get_output_pos() const override {
		return written;
	}
	
private:
	BinaryData* out = 0;
	size_t written = 0;
	
};

}
}

#endif // INCLUDE_VNX_WEB_BINARYDATAOUTPUTSTREAM_H
