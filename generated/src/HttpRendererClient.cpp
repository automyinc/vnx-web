
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/HttpRendererClient.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>


namespace vnx {
namespace web {

HttpRendererClient::HttpRendererClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

HttpRendererClient::HttpRendererClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

void HttpRendererClient::handle(const ::std::shared_ptr<const ::vnx::web::HttpResponse>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0xe3324942782bc338ull));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void HttpRendererClient::handle_async(const ::std::shared_ptr<const ::vnx::web::HttpResponse>& sample) {
	vnx_is_async = true;
	handle(sample);
}

void HttpRendererClient::handle(const ::std::shared_ptr<const ::vnx::web::StreamEventArray>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0x30751ebcba8fcb3full));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void HttpRendererClient::handle_async(const ::std::shared_ptr<const ::vnx::web::StreamEventArray>& sample) {
	vnx_is_async = true;
	handle(sample);
}


} // namespace vnx
} // namespace web