
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/HttpProcessorClient.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>


namespace vnx {
namespace web {

HttpProcessorClient::HttpProcessorClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

HttpProcessorClient::HttpProcessorClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

void HttpProcessorClient::handle(const ::std::shared_ptr<const ::vnx::web::HttpRequest>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0x2e06b7981ed22c1dull));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void HttpProcessorClient::handle_async(const ::std::shared_ptr<const ::vnx::web::HttpRequest>& sample) {
	vnx_is_async = true;
	handle(sample);
}

void HttpProcessorClient::handle(const ::std::shared_ptr<const ::vnx::web::Response>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0x84c27382aa993259ull));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void HttpProcessorClient::handle_async(const ::std::shared_ptr<const ::vnx::web::Response>& sample) {
	vnx_is_async = true;
	handle(sample);
}

void HttpProcessorClient::handle(const ::std::shared_ptr<const ::vnx::web::StreamEvent>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0x63045519a77fbf1bull));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void HttpProcessorClient::handle_async(const ::std::shared_ptr<const ::vnx::web::StreamEvent>& sample) {
	vnx_is_async = true;
	handle(sample);
}


} // namespace vnx
} // namespace web