
#ifndef INCLUDE_VNX_WEB_HTTPRENDERER_H
#define INCLUDE_VNX_WEB_HTTPRENDERER_H

#include <vnx/web/HttpRendererBase.hxx>

#include <unordered_map>


namespace vnx {
namespace web {

class HttpRenderer : public HttpRendererBase {
public:
	HttpRenderer(const std::string& _vnx_name);
	
protected:
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::HttpResponse> response) override;
	
	void handle(std::shared_ptr<const ::vnx::web::StreamEventArray> events) override;
	
private:
	void render(vnx::OutputBuffer& out, const char str[]);
	
	void render(vnx::OutputBuffer& out, const std::string& str);
	
	void render(vnx::OutputBuffer& out, std::shared_ptr<const Content> content);
	
	void render(vnx::OutputBuffer& out, std::shared_ptr<const File> file);
	
	void render(vnx::OutputBuffer& out, std::shared_ptr<const Directory> directory);
	
	void render_header(vnx::OutputBuffer &out, const std::string &key, const std::string &value);
	
	void render_header(vnx::OutputBuffer &out, const std::pair<std::string, std::string> &field);
	
private:
	std::unordered_map<Hash128, int64_t> state_map;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_HTTPRENDERER_H
