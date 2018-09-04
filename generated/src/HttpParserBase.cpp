
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/HttpParserBase.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>
#include <vnx/Config.h>


namespace vnx {
namespace web {


const vnx::Hash64 HttpParserBase::VNX_TYPE_HASH(0x5b09fdf93e688c4full);
const vnx::Hash64 HttpParserBase::VNX_CODE_HASH(0x1fb59b621937d781ull);

HttpParserBase::HttpParserBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".input", input);
	vnx::read_config(vnx_name + ".output", output);
	vnx::read_config(vnx_name + ".max_request_size", max_request_size);
}

vnx::Hash64 HttpParserBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* HttpParserBase::get_type_name() const {
	return "vnx.web.HttpParser";
}

void HttpParserBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, input);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, output);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, max_request_size);
	_visitor.type_end(*_type_code);
}

void HttpParserBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"input\": "; vnx::write(_out, input);
	_out << ", \"output\": "; vnx::write(_out, output);
	_out << ", \"max_request_size\": "; vnx::write(_out, max_request_size);
	_out << "}";
}

void HttpParserBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "input") {
			vnx::from_string(_entry.second, input);
		} else if(_entry.first == "output") {
			vnx::from_string(_entry.second, output);
		} else if(_entry.first == "max_request_size") {
			vnx::from_string(_entry.second, max_request_size);
		}
	}
}

vnx::Object HttpParserBase::to_object() const {
	vnx::Object _object;
	_object["input"] = input;
	_object["output"] = output;
	_object["max_request_size"] = max_request_size;
	return _object;
}

void HttpParserBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "input") {
			_entry.second.to(input);
		} else if(_entry.first == "output") {
			_entry.second.to(output);
		} else if(_entry.first == "max_request_size") {
			_entry.second.to(max_request_size);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const HttpParserBase& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, HttpParserBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* HttpParserBase::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x5b09fdf93e688c4full));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> HttpParserBase::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.HttpParser";
	type_code->type_hash = vnx::Hash64(0x5b09fdf93e688c4full);
	type_code->code_hash = vnx::Hash64(0x1fb59b621937d781ull);
	type_code->methods.resize(2);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_StreamEventArray";
		call_type->type_hash = vnx::Hash64(0x30751ebcba8fcb3full);
		call_type->code_hash = vnx::Hash64(0x44f11292a65f0ad8ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.HttpParser.handle_vnx_web_StreamEventArray.return";
			return_type->type_hash = vnx::Hash64(0x6f17c6864e97d8b1ull);
			return_type->code_hash = vnx::Hash64(0xe0dbbc0295fa9772ull);
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
		call_type->name = "vnx.web.handle_vnx_web_StreamRead";
		call_type->type_hash = vnx::Hash64(0xeb7fd1f797fce2dbull);
		call_type->code_hash = vnx::Hash64(0x29fdf14137c09a95ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.HttpParser.handle_vnx_web_StreamRead.return";
			return_type->type_hash = vnx::Hash64(0x14a2fb3d6564a8b5ull);
			return_type->code_hash = vnx::Hash64(0x21e089b5faaddc79ull);
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
	type_code->fields.resize(3);
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
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "max_request_size";
		field.value = vnx::to_string(4194304);
		field.code = {8};
	}
	type_code->build();
	return type_code;
}

void HttpParserBase::handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0x88743c1ec13df063ull) {
		std::shared_ptr<const vnx::web::StreamEventArray> _value = std::dynamic_pointer_cast<const vnx::web::StreamEventArray>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x538531a025cd468aull) {
		std::shared_ptr<const vnx::web::StreamRead> _value = std::dynamic_pointer_cast<const vnx::web::StreamRead>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	}
}

bool HttpParserBase::call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type) {
	if(_call_type->type_hash == vnx::Hash64(0x30751ebcba8fcb3full)) {
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
	} else if(_call_type->type_hash == vnx::Hash64(0xeb7fd1f797fce2dbull)) {
		::std::shared_ptr<const ::vnx::web::StreamRead> sample;
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

void read(TypeInput& in, ::vnx::web::HttpParserBase& value, const TypeCode* type_code, const uint16_t* code) {
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
		const vnx::TypeField* const _field = type_code->field_map[2];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.max_request_size, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.input, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.output, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::HttpParserBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::HttpParserBase>(out);
		vnx::write_class_header<::vnx::web::HttpParserBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.max_request_size);
	vnx::write(out, value.input, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.output, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::web::HttpParserBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::HttpParserBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::HttpParserBase& value) {
	value.accept(visitor);
}

} // vnx
