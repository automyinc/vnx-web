
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/CacheClient.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>


namespace vnx {
namespace web {

CacheClient::CacheClient(const std::string& service_name)
	:	Client::Client(vnx::Hash64(service_name))
{
}

CacheClient::CacheClient(vnx::Hash64 service_addr)
	:	Client::Client(service_addr)
{
}

void CacheClient::handle(const ::std::shared_ptr<const ::vnx::web::Provider>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0xb3ca09bf0c4d5dadull));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void CacheClient::handle_async(const ::std::shared_ptr<const ::vnx::web::Provider>& sample) {
	vnx_is_async = true;
	handle(sample);
}

void CacheClient::handle(const ::std::shared_ptr<const ::vnx::web::Request>& sample) {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0x4da6e3f631b8d157ull));
	{
		vnx::write(_out, sample, _type_code, _type_code->fields[0].code.data());
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void CacheClient::handle_async(const ::std::shared_ptr<const ::vnx::web::Request>& sample) {
	vnx_is_async = true;
	handle(sample);
}

void CacheClient::handle(const ::std::shared_ptr<const ::vnx::web::Response>& sample) {
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

void CacheClient::handle_async(const ::std::shared_ptr<const ::vnx::web::Response>& sample) {
	vnx_is_async = true;
	handle(sample);
}

void CacheClient::purge() {
	std::shared_ptr<vnx::Binary> _argument_data = vnx::Binary::create();
	vnx::BinaryOutputStream _stream_out(_argument_data.get());
	vnx::TypeOutput _out(&_stream_out);
	const vnx::TypeCode* _type_code = vnx::get_type_code(vnx::Hash64(0x12a031b7ab016698ull));
	{
	}
	_out.flush();
	_argument_data->type_code = _type_code;
	vnx_request(_argument_data);
}

void CacheClient::purge_async() {
	vnx_is_async = true;
	purge();
}


} // namespace vnx
} // namespace web
