
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/Content.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 Content::VNX_TYPE_HASH(0x461d3a7c5b20db5full);
const vnx::Hash64 Content::VNX_CODE_HASH(0x30607c6bc8f589afull);

vnx::Hash64 Content::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* Content::get_type_name() const {
	return "vnx.web.Content";
}

std::shared_ptr<Content> Content::create() {
	return std::make_shared<Content>();
}

std::shared_ptr<vnx::Value> Content::clone() const {
	return std::make_shared<Content>(*this);
}

void Content::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Content::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Content::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, mime_type);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, time_stamp_ms);
	_visitor.type_end(*_type_code);
}

void Content::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"mime_type\": "; vnx::write(_out, mime_type);
	_out << ", \"time_stamp_ms\": "; vnx::write(_out, time_stamp_ms);
	_out << "}";
}

void Content::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "mime_type") {
			vnx::from_string(_entry.second, mime_type);
		} else if(_entry.first == "time_stamp_ms") {
			vnx::from_string(_entry.second, time_stamp_ms);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const Content& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, Content& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Content::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x461d3a7c5b20db5full));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Content::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.Content";
	type_code->type_hash = vnx::Hash64(0x461d3a7c5b20db5full);
	type_code->code_hash = vnx::Hash64(0x30607c6bc8f589afull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Content>(); };
	type_code->fields.resize(2);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "mime_type";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "time_stamp_ms";
		field.code = {8};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::Content& value, const TypeCode* type_code, const uint16_t* code) {
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
		const vnx::TypeField* const _field = type_code->field_map[1];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.time_stamp_ms, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.mime_type, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::Content& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::Content>(out);
		vnx::write_class_header<::vnx::web::Content>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.time_stamp_ms);
	vnx::write(out, value.mime_type, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::web::Content& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::Content& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::Content& value) {
	value.accept(visitor);
}

} // vnx
