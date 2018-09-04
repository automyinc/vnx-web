
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/HttpRendererBase.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>
#include <vnx/Config.h>


namespace vnx {
namespace web {


const vnx::Hash64 HttpRendererBase::VNX_TYPE_HASH(0x809feb631616e83full);
const vnx::Hash64 HttpRendererBase::VNX_CODE_HASH(0xb27c40db3a6111f9ull);

HttpRendererBase::HttpRendererBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".input", input);
	vnx::read_config(vnx_name + ".output", output);
}

vnx::Hash64 HttpRendererBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* HttpRendererBase::get_type_name() const {
	return "vnx.web.HttpRenderer";
}

void HttpRendererBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, input);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, output);
	_visitor.type_end(*_type_code);
}

void HttpRendererBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"input\": "; vnx::write(_out, input);
	_out << ", \"output\": "; vnx::write(_out, output);
	_out << "}";
}

void HttpRendererBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "input") {
			vnx::from_string(_entry.second, input);
		} else if(_entry.first == "output") {
			vnx::from_string(_entry.second, output);
		}
	}
}

vnx::Object HttpRendererBase::to_object() const {
	vnx::Object _object;
	_object["input"] = input;
	_object["output"] = output;
	return _object;
}

void HttpRendererBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "input") {
			_entry.second.to(input);
		} else if(_entry.first == "output") {
			_entry.second.to(output);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const HttpRendererBase& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, HttpRendererBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* HttpRendererBase::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x809feb631616e83full));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> HttpRendererBase::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.HttpRenderer";
	type_code->type_hash = vnx::Hash64(0x809feb631616e83full);
	type_code->code_hash = vnx::Hash64(0xb27c40db3a6111f9ull);
	type_code->methods.resize(2);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_HttpResponse";
		call_type->type_hash = vnx::Hash64(0xe3324942782bc338ull);
		call_type->code_hash = vnx::Hash64(0xb05cfa0e4ad399f7ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.HttpRenderer.handle_vnx_web_HttpResponse.return";
			return_type->type_hash = vnx::Hash64(0xff574d3812c39572ull);
			return_type->code_hash = vnx::Hash64(0x23aec3b9b9effef0ull);
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
		call_type->name = "vnx.web.handle_vnx_web_StreamEventArray";
		call_type->type_hash = vnx::Hash64(0x30751ebcba8fcb3full);
		call_type->code_hash = vnx::Hash64(0x44f11292a65f0ad8ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.HttpRenderer.handle_vnx_web_StreamEventArray.return";
			return_type->type_hash = vnx::Hash64(0x8705db4a1f8853bfull);
			return_type->code_hash = vnx::Hash64(0x4715a22c8f9e93f0ull);
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
	type_code->fields.resize(2);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "input";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "output";
		field.code = {12, 5};
	}
	type_code->build();
	return type_code;
}

void HttpRendererBase::handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0x5f05cae7dd441511ull) {
		std::shared_ptr<const vnx::web::HttpResponse> _value = std::dynamic_pointer_cast<const vnx::web::HttpResponse>(_sample->value);
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

bool HttpRendererBase::call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type) {
	if(_call_type->type_hash == vnx::Hash64(0xe3324942782bc338ull)) {
		::std::shared_ptr<const ::vnx::web::HttpResponse> sample;
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

void read(TypeInput& in, ::vnx::web::HttpRendererBase& value, const TypeCode* type_code, const uint16_t* code) {
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
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.input, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.output, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::HttpRendererBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::HttpRendererBase>(out);
		vnx::write_class_header<::vnx::web::HttpRendererBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.input, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.output, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::web::HttpRendererBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::HttpRendererBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::HttpRendererBase& value) {
	value.accept(visitor);
}

} // vnx
