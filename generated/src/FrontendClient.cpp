
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/FrontendClient.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>


namespace vnx {
namespace web {

FrontendClient::FrontendClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

FrontendClient::FrontendClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

void FrontendClient::handle(const ::std::shared_ptr<const ::vnx::web::StreamEventArray>& sample) {
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

void FrontendClient::handle_async(const ::std::shared_ptr<const ::vnx::web::StreamEventArray>& sample) {
	vnx_is_async = true;
	handle(sample);
}

void FrontendClient::handle(const ::std::shared_ptr<const ::vnx::web::StreamWrite>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0xc1497e9e245cdacdull));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void FrontendClient::handle_async(const ::std::shared_ptr<const ::vnx::web::StreamWrite>& sample) {
	vnx_is_async = true;
	handle(sample);
}


} // namespace vnx
} // namespace web