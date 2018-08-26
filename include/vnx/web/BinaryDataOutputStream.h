
#ifndef INCLUDE_VNX_WEB_BINARYDATAOUTPUTSTREAM_H
#define INCLUDE_VNX_WEB_BINARYDATAOUTPUTSTREAM_H

#include <vnx/web/BinaryData.hxx>
#include <vnx/Output.h>


namespace vnx {
namespace web {

class BinaryDataOutputStream : public vnx::OutputStream {
public:
	BinaryDataOutputStream(BinaryData* data_) : data(data_) {}
	
	void write(const void* buf, size_t len) override {
		data->chunks.emplace_back(len);
		::memcpy(data->chunks.back().data(), buf, len);
		data->chunks.back().set_size(len);
		written += len;
	}
	
	size_t get_output_pos() const override {
		return written;
	}
	
private:
	BinaryData* data = 0;
	size_t written = 0;
	
};

} // web
} // vnx

#endif // INCLUDE_VNX_WEB_BINARYDATAOUTPUTSTREAM_H
