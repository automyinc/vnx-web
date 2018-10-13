
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/SetCookie.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 SetCookie::VNX_TYPE_HASH(0x2202a52496599d1cull);
const vnx::Hash64 SetCookie::VNX_CODE_HASH(0x43e4a6dc0beaf7afull);

vnx::Hash64 SetCookie::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* SetCookie::get_type_name() const {
	return "vnx.web.SetCookie";
}

std::shared_ptr<SetCookie> SetCookie::create() {
	return std::make_shared<SetCookie>();
}

std::shared_ptr<vnx::Value> SetCookie::clone() const {
	return std::make_shared<SetCookie>(*this);
}

void SetCookie::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void SetCookie::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void SetCookie::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, code);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, message);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, name);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, value);
	_visitor.type_end(*_type_code);
}

void SetCookie::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"code\": "; vnx::write(_out, code);
	_out << ", \"message\": "; vnx::write(_out, message);
	_out << ", \"name\": "; vnx::write(_out, name);
	_out << ", \"value\": "; vnx::write(_out, value);
	_out << "}";
}

void SetCookie::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "code") {
			vnx::from_string(_entry.second, code);
		} else if(_entry.first == "message") {
			vnx::from_string(_entry.second, message);
		} else if(_entry.first == "name") {
			vnx::from_string(_entry.second, name);
		} else if(_entry.first == "value") {
			vnx::from_string(_entry.second, value);
		}
	}
}

vnx::Object SetCookie::to_object() const {
	vnx::Object _object;
	_object["code"] = code;
	_object["message"] = message;
	_object["name"] = name;
	_object["value"] = value;
	return _object;
}

void SetCookie::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "code") {
			_entry.second.to(code);
		} else if(_entry.first == "message") {
			_entry.second.to(message);
		} else if(_entry.first == "name") {
			_entry.second.to(name);
		} else if(_entry.first == "value") {
			_entry.second.to(value);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const SetCookie& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, SetCookie& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* SetCookie::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x2202a52496599d1cull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> SetCookie::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.SetCookie";
	type_code->type_hash = vnx::Hash64(0x2202a52496599d1cull);
	type_code->code_hash = vnx::Hash64(0x43e4a6dc0beaf7afull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::web::ErrorCode::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<SetCookie>(); };
	type_code->fields.resize(4);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "code";
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "message";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "name";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "value";
		field.code = {12, 5};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::SetCookie& value, const TypeCode* type_code, const uint16_t* code) {
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
		const vnx::TypeField* const _field = type_code->field_map[0];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.code, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 1: vnx::read(in, value.message, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.name, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.value, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::SetCookie& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::SetCookie>(out);
		vnx::write_class_header<::vnx::web::SetCookie>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.code);
	vnx::write(out, value.message, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.name, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.value, type_code, type_code->fields[3].code.data());
}

void read(std::istream& in, ::vnx::web::SetCookie& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::SetCookie& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::SetCookie& value) {
	value.accept(visitor);
}

} // vnx
