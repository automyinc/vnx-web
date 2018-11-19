
#include <vnx/web/HttpRenderer.h>
#include <vnx/web/File.hxx>
#include <vnx/web/Directory.hxx>
#include <vnx/web/StreamWrite.hxx>
#include <vnx/web/ErrorCode.hxx>

#include <sstream>


namespace vnx {
namespace web {

HttpRenderer::HttpRenderer(const std::string& _vnx_name)
		: HttpRendererBase(_vnx_name)
{
}

void HttpRenderer::main() {
	
	subscribe(input);
	
	Super::main();
}

inline void render(vnx::OutputBuffer& out, const char str[]) {
	out.write(str, ::strlen(str));
}

inline void render(vnx::OutputBuffer& out, const std::string& str) {
	out.write(str.c_str(), str.size());
}

inline void render_header(vnx::OutputBuffer &out, const std::string &key, const std::string &value) {
	render(out, key);
	render(out, ": ");
	render(out, value);
	render(out, "\r\n");
}

void HttpRenderer::handle(std::shared_ptr<const ::vnx::web::HttpResponse> response) {
	
	auto iter = state_map.find(response->stream);
	if(iter == state_map.end()) {
		if(response->sequence > 1) {
			return;
		}
	}
	
	std::shared_ptr<StreamWrite> sample = StreamWrite::create();
	sample->stream = response->stream;
	sample->is_eof = response->do_close;
	
	int64_t& state = state_map[response->stream];
	if(response->sequence != state + 1) {
		sample->is_eof = true;
		publish(sample, output);
		return;
	}
	state = response->sequence;
	
	MemoryOutputStream stream(&sample->data);
	OutputBuffer out(&stream);
	
	render(out, "HTTP/1.1 ");
	render(out, std::to_string(response->status));
	render(out, " ");
	render(out, ErrorCode::get_error_string(response->status));
	render(out, "\r\n");
	
	for(const auto& field : response->header) {
		render_header(out, field.first, field.second);
	}
	
	// TODO: handle big files as chunks
	
	{
		auto file = std::dynamic_pointer_cast<const File>(response->result);
		if(file) {
			render_header(out, "Content-Length", std::to_string(file->get_num_bytes()));
			render_header(out, "Content-Type", file->mime_type);
			render(out, "\r\n");
			if(!response->is_head_response) {
				out.write(file->data.data(), file->data.size());
			}
		} else {
			render_header(out, "Content-Length", "0");
			render(out, "\r\n");
		}
	}
	
	out.flush();
	
	publish(sample, output, BLOCKING);
}

void HttpRenderer::handle(std::shared_ptr<const ::vnx::web::StreamEventArray> events) {
	for(const stream_event_t& event : events->array) {
		switch(event.event) {
			case stream_event_t::EVENT_EOF:
				state_map.erase(event.stream);
				break;
		}
	}
}


} // web
} // vnx
