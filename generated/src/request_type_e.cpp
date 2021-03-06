
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/request_type_e.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 request_type_e::VNX_TYPE_HASH(0xbdff18af3e34999eull);
const vnx::Hash64 request_type_e::VNX_CODE_HASH(0x7325cd53b4cb23baull);

vnx::Hash64 request_type_e::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* request_type_e::get_type_name() const {
	return "vnx.web.request_type_e";
}

std::shared_ptr<request_type_e> request_type_e::create() {
	return std::make_shared<request_type_e>();
}

std::shared_ptr<request_type_e> request_type_e::clone() const {
	return std::make_shared<request_type_e>(*this);
}

void request_type_e::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void request_type_e::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void request_type_e::accept(vnx::Visitor& _visitor) const {
	std::string _name;
	switch(value) {
		case GENERIC: _name = "GENERIC"; break;
		case READ: _name = "READ"; break;
		case WRITE: _name = "WRITE"; break;
	}
	_visitor.enum_value(value, _name);
}

void request_type_e::write(std::ostream& _out) const {
	switch(value) {
		case GENERIC: _out << "\"GENERIC\""; break;
		case READ: _out << "\"READ\""; break;
		case WRITE: _out << "\"WRITE\""; break;
		default: _out << value;
	}
}

void request_type_e::read(std::istream& _in) {
	std::string _name;
	vnx::read(_in, _name);
	if(_name == "GENERIC") value = 3563222431;
	else if(_name == "READ") value = 329112197;
	else if(_name == "WRITE") value = 1285543041;
	else value = std::atoi(_name.c_str());
}

vnx::Object request_type_e::to_object() const {
	vnx::Object _object;
	_object["value"] = value;
	return _object;
}

void request_type_e::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "value") {
			_entry.second.to(value);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const request_type_e& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, request_type_e& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* request_type_e::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xbdff18af3e34999eull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> request_type_e::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.request_type_e";
	type_code->type_hash = vnx::Hash64(0xbdff18af3e34999eull);
	type_code->code_hash = vnx::Hash64(0x7325cd53b4cb23baull);
	type_code->is_enum = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<request_type_e>>(); };
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "value";
		field.code = {3};
	}
	type_code->enum_map[3563222431] = "GENERIC";
	type_code->enum_map[329112197] = "READ";
	type_code->enum_map[1285543041] = "WRITE";
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::request_type_e& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.value, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::request_type_e& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::request_type_e>(out);
		vnx::write_class_header<::vnx::web::request_type_e>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.value);
}

void read(std::istream& in, ::vnx::web::request_type_e& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::request_type_e& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::request_type_e& value) {
	value.accept(visitor);
}

} // vnx
