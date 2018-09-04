
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/StreamRead.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 StreamRead::VNX_TYPE_HASH(0x538531a025cd468aull);
const vnx::Hash64 StreamRead::VNX_CODE_HASH(0xc0ef35722a3074cull);

vnx::Hash64 StreamRead::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* StreamRead::get_type_name() const {
	return "vnx.web.StreamRead";
}

std::shared_ptr<StreamRead> StreamRead::create() {
	return std::make_shared<StreamRead>();
}

std::shared_ptr<vnx::Value> StreamRead::clone() const {
	return std::make_shared<StreamRead>(*this);
}

void StreamRead::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void StreamRead::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void StreamRead::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, stream);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, data);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, channel);
	_visitor.type_end(*_type_code);
}

void StreamRead::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"stream\": "; vnx::write(_out, stream);
	_out << ", \"data\": "; vnx::write(_out, data);
	_out << ", \"channel\": "; vnx::write(_out, channel);
	_out << "}";
}

void StreamRead::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "stream") {
			vnx::from_string(_entry.second, stream);
		} else if(_entry.first == "data") {
			vnx::from_string(_entry.second, data);
		} else if(_entry.first == "channel") {
			vnx::from_string(_entry.second, channel);
		}
	}
}

vnx::Object StreamRead::to_object() const {
	vnx::Object _object;
	_object["stream"] = stream;
	_object["data"] = data;
	_object["channel"] = channel;
	return _object;
}

void StreamRead::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "stream") {
			_entry.second.to(stream);
		} else if(_entry.first == "data") {
			_entry.second.to(data);
		} else if(_entry.first == "channel") {
			_entry.second.to(channel);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const StreamRead& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, StreamRead& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* StreamRead::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x538531a025cd468aull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> StreamRead::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.StreamRead";
	type_code->type_hash = vnx::Hash64(0x538531a025cd468aull);
	type_code->code_hash = vnx::Hash64(0xc0ef35722a3074cull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<StreamRead>(); };
	type_code->fields.resize(3);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "stream";
		field.code = {11, 2, 4};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "data";
		field.code = {12, 1};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "channel";
		field.code = {12, 5};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::StreamRead& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.stream, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.data, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.channel, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::StreamRead& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::StreamRead>(out);
		vnx::write_class_header<::vnx::web::StreamRead>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.stream, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.data, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.channel, type_code, type_code->fields[2].code.data());
}

void read(std::istream& in, ::vnx::web::StreamRead& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::StreamRead& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::StreamRead& value) {
	value.accept(visitor);
}

} // vnx
