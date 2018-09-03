
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/HttpProcessorBase.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>
#include <vnx/Config.h>


namespace vnx {
namespace web {


const vnx::Hash64 HttpProcessorBase::VNX_TYPE_HASH(0xea3bdb551f410546ull);
const vnx::Hash64 HttpProcessorBase::VNX_CODE_HASH(0x36a1a6d8586f9a04ull);

HttpProcessorBase::HttpProcessorBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".input", input);
	vnx::read_config(vnx_name + ".channel", channel);
	vnx::read_config(vnx_name + ".output", output);
	vnx::read_config(vnx_name + ".default_domain", default_domain);
	vnx::read_config(vnx_name + ".domain_map", domain_map);
	vnx::read_config(vnx_name + ".index_path", index_path);
	vnx::read_config(vnx_name + ".max_queue_size", max_queue_size);
	vnx::read_config(vnx_name + ".max_pending", max_pending);
	vnx::read_config(vnx_name + ".timeout_ms", timeout_ms);
	vnx::read_config(vnx_name + ".keepalive", keepalive);
	vnx::read_config(vnx_name + ".max_num_pages", max_num_pages);
}

vnx::Hash64 HttpProcessorBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* HttpProcessorBase::get_type_name() const {
	return "vnx.web.HttpProcessor";
}

void HttpProcessorBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, input);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, channel);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, output);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, default_domain);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, domain_map);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, index_path);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, max_queue_size);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, max_pending);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, timeout_ms);
	_visitor.type_field(_type_code->fields[9], 9); vnx::accept(_visitor, keepalive);
	_visitor.type_field(_type_code->fields[10], 10); vnx::accept(_visitor, max_num_pages);
	_visitor.type_end(*_type_code);
}

void HttpProcessorBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"input\": "; vnx::write(_out, input);
	_out << ", \"channel\": "; vnx::write(_out, channel);
	_out << ", \"output\": "; vnx::write(_out, output);
	_out << ", \"default_domain\": "; vnx::write(_out, default_domain);
	_out << ", \"domain_map\": "; vnx::write(_out, domain_map);
	_out << ", \"index_path\": "; vnx::write(_out, index_path);
	_out << ", \"max_queue_size\": "; vnx::write(_out, max_queue_size);
	_out << ", \"max_pending\": "; vnx::write(_out, max_pending);
	_out << ", \"timeout_ms\": "; vnx::write(_out, timeout_ms);
	_out << ", \"keepalive\": "; vnx::write(_out, keepalive);
	_out << ", \"max_num_pages\": "; vnx::write(_out, max_num_pages);
	_out << "}";
}

void HttpProcessorBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "input") {
			vnx::from_string(_entry.second, input);
		} else if(_entry.first == "channel") {
			vnx::from_string(_entry.second, channel);
		} else if(_entry.first == "output") {
			vnx::from_string(_entry.second, output);
		} else if(_entry.first == "default_domain") {
			vnx::from_string(_entry.second, default_domain);
		} else if(_entry.first == "domain_map") {
			vnx::from_string(_entry.second, domain_map);
		} else if(_entry.first == "index_path") {
			vnx::from_string(_entry.second, index_path);
		} else if(_entry.first == "max_queue_size") {
			vnx::from_string(_entry.second, max_queue_size);
		} else if(_entry.first == "max_pending") {
			vnx::from_string(_entry.second, max_pending);
		} else if(_entry.first == "timeout_ms") {
			vnx::from_string(_entry.second, timeout_ms);
		} else if(_entry.first == "keepalive") {
			vnx::from_string(_entry.second, keepalive);
		} else if(_entry.first == "max_num_pages") {
			vnx::from_string(_entry.second, max_num_pages);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const HttpProcessorBase& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, HttpProcessorBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* HttpProcessorBase::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xea3bdb551f410546ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> HttpProcessorBase::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.HttpProcessor";
	type_code->type_hash = vnx::Hash64(0xea3bdb551f410546ull);
	type_code->code_hash = vnx::Hash64(0x36a1a6d8586f9a04ull);
	type_code->methods.resize(3);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_HttpRequest";
		call_type->type_hash = vnx::Hash64(0x2e06b7981ed22c1dull);
		call_type->code_hash = vnx::Hash64(0x6e7b6dede2b1e13dull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.HttpProcessor.handle_vnx_web_HttpRequest.return";
			return_type->type_hash = vnx::Hash64(0x621614f5b9a1e366ull);
			return_type->code_hash = vnx::Hash64(0x77e16b4befe4b4d3ull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "sample";
			field.code = {16};
		}
		call_type->build();
		type_code->methods[0] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_Response";
		call_type->type_hash = vnx::Hash64(0x84c27382aa993259ull);
		call_type->code_hash = vnx::Hash64(0x508ce90e1157650cull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.HttpProcessor.handle_vnx_web_Response.return";
			return_type->type_hash = vnx::Hash64(0x72113b5ecd092028ull);
			return_type->code_hash = vnx::Hash64(0x4a05061122c247cull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "sample";
			field.code = {16};
		}
		call_type->build();
		type_code->methods[1] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_StreamEventArray";
		call_type->type_hash = vnx::Hash64(0x30751ebcba8fcb3full);
		call_type->code_hash = vnx::Hash64(0x44f11292a65f0ad8ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.HttpProcessor.handle_vnx_web_StreamEventArray.return";
			return_type->type_hash = vnx::Hash64(0x867d573926223ccdull);
			return_type->code_hash = vnx::Hash64(0x7a33783152651d4full);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "sample";
			field.code = {16};
		}
		call_type->build();
		type_code->methods[2] = vnx::register_type_code(call_type);
	}
	type_code->fields.resize(11);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "input";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "channel";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "output";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "default_domain";
		field.value = vnx::to_string("test.com");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "domain_map";
		field.code = {13, 4, 12, 5, 12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "index_path";
		field.value = vnx::to_string("/default/file/");
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "max_queue_size";
		field.value = vnx::to_string(100);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "max_pending";
		field.value = vnx::to_string(100000);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[8];
		field.name = "timeout_ms";
		field.value = vnx::to_string(1000);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[9];
		field.name = "keepalive";
		field.value = vnx::to_string(true);
		field.code = {1};
	}
	{
		vnx::TypeField& field = type_code->fields[10];
		field.name = "max_num_pages";
		field.value = vnx::to_string(10000);
		field.code = {8};
	}
	type_code->build();
	return type_code;
}

void HttpProcessorBase::handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0x84bd8b34f037a33eull) {
		std::shared_ptr<const vnx::web::HttpRequest> _value = std::dynamic_pointer_cast<const vnx::web::HttpRequest>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x5f05cae7dd441511ull) {
		std::shared_ptr<const vnx::web::HttpResponse> _value = std::dynamic_pointer_cast<const vnx::web::HttpResponse>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x2a23ede4ff56e0a2ull) {
		std::shared_ptr<const vnx::web::Response> _value = std::dynamic_pointer_cast<const vnx::web::Response>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x88743c1ec13df063ull) {
		std::shared_ptr<const vnx::web::StreamEventArray> _value = std::dynamic_pointer_cast<const vnx::web::StreamEventArray>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	}
}

bool HttpProcessorBase::call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type) {
	if(_call_type->type_hash == vnx::Hash64(0x2e06b7981ed22c1dull)) {
		::std::shared_ptr<const ::vnx::web::HttpRequest> sample;
		const char* const _buf = _in.read(_call_type->total_field_size);
		for(const vnx::TypeField* _field : _call_type->ext_fields) {
			switch(_field->native_index) {
				case 0: vnx::read(_in, sample, _call_type, _field->code.data()); break;
				default: vnx::skip(_in, _call_type, _field->code.data());
			}
		}
		handle(sample);
		return true;
	} else if(_call_type->type_hash == vnx::Hash64(0x84c27382aa993259ull)) {
		::std::shared_ptr<const ::vnx::web::Response> sample;
		const char* const _buf = _in.read(_call_type->total_field_size);
		for(const vnx::TypeField* _field : _call_type->ext_fields) {
			switch(_field->native_index) {
				case 0: vnx::read(_in, sample, _call_type, _field->code.data()); break;
				default: vnx::skip(_in, _call_type, _field->code.data());
			}
		}
		handle(sample);
		return true;
	} else if(_call_type->type_hash == vnx::Hash64(0x30751ebcba8fcb3full)) {
		::std::shared_ptr<const ::vnx::web::StreamEventArray> sample;
		const char* const _buf = _in.read(_call_type->total_field_size);
		for(const vnx::TypeField* _field : _call_type->ext_fields) {
			switch(_field->native_index) {
				case 0: vnx::read(_in, sample, _call_type, _field->code.data()); break;
				default: vnx::skip(_in, _call_type, _field->code.data());
			}
		}
		handle(sample);
		return true;
	}
	return false;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::HttpProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code) {
		throw std::logic_error("read(): type_code == 0");
	}
	if(code) {
		switch(code[0]) {
			case CODE_STRUCT: type_code = type_code->depends[code[1]]; break;
			case CODE_ALT_STRUCT: type_code = type_code->depends[vnx::flip_bytes(code[1])]; break;
			default: vnx::skip(in, type_code, code); return;
		}
	}
	const char* const _buf = in.read(type_code->total_field_size);
	{
		const vnx::TypeField* const _field = type_code->field_map[6];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.max_queue_size, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[7];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.max_pending, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[8];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.timeout_ms, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[9];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.keepalive, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[10];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.max_num_pages, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.input, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.channel, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.output, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.default_domain, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.domain_map, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.index_path, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::HttpProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::HttpProcessorBase>(out);
		vnx::write_class_header<::vnx::web::HttpProcessorBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(33);
	vnx::write_value(_buf + 0, value.max_queue_size);
	vnx::write_value(_buf + 8, value.max_pending);
	vnx::write_value(_buf + 16, value.timeout_ms);
	vnx::write_value(_buf + 24, value.keepalive);
	vnx::write_value(_buf + 25, value.max_num_pages);
	vnx::write(out, value.input, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.channel, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.output, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.default_domain, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.domain_map, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.index_path, type_code, type_code->fields[5].code.data());
}

void read(std::istream& in, ::vnx::web::HttpProcessorBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::HttpProcessorBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::HttpProcessorBase& value) {
	value.accept(visitor);
}

} // vnx
