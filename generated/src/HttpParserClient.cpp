
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/HttpParserClient.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>


namespace vnx {
namespace web {

HttpParserClient::HttpParserClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

HttpParserClient::HttpParserClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

void HttpParserClient::handle(const ::std::shared_ptr<const ::vnx::web::StreamEvent>& sample) {
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

void HttpParserClient::handle_async(const ::std::shared_ptr<const ::vnx::web::StreamEvent>& sample) {
	vnx_is_async = true;
	handle(sample);
}

void HttpParserClient::handle(const ::std::shared_ptr<const ::vnx::web::StreamRead>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0xeb7fd1f797fce2dbull));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void HttpParserClient::handle_async(const ::std::shared_ptr<const ::vnx::web::StreamRead>& sample) {
	vnx_is_async = true;
	handle(sample);
}


} // namespace vnx
} // namespace web