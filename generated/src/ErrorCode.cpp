
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/ErrorCode.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {

const ::int32_t ErrorCode::MOVED_PERMANENTLY;
const ::int32_t ErrorCode::MOVED_TEMPORARILY;
const ::int32_t ErrorCode::BAD_REQUEST;
const ::int32_t ErrorCode::FORBIDDEN;
const ::int32_t ErrorCode::NOT_FOUND;
const ::int32_t ErrorCode::TIMEOUT;
const ::int32_t ErrorCode::INTERNAL_ERROR;
const ::int32_t ErrorCode::OVERLOAD;

const vnx::Hash64 ErrorCode::VNX_TYPE_HASH(0x91aec7d2ce28e5e3ull);
const vnx::Hash64 ErrorCode::VNX_CODE_HASH(0xe519ccdb44a014bdull);

vnx::Hash64 ErrorCode::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* ErrorCode::get_type_name() const {
	return "vnx.web.ErrorCode";
}

std::shared_ptr<ErrorCode> ErrorCode::create() {
	return std::make_shared<ErrorCode>();
}

std::shared_ptr<vnx::Value> ErrorCode::clone() const {
	return std::make_shared<ErrorCode>(*this);
}

void ErrorCode::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void ErrorCode::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void ErrorCode::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, code);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, message);
	_visitor.type_end(*_type_code);
}

void ErrorCode::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"code\": "; vnx::write(_out, code);
	_out << ", \"message\": "; vnx::write(_out, message);
	_out << "}";
}

void ErrorCode::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "code") {
			vnx::from_string(_entry.second, code);
		} else if(_entry.first == "message") {
			vnx::from_string(_entry.second, message);
		}
	}
}

vnx::Object ErrorCode::to_object() const {
	vnx::Object _object;
	_object["code"] = code;
	_object["message"] = message;
	return _object;
}

void ErrorCode::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "code") {
			_entry.second.to(code);
		} else if(_entry.first == "message") {
			_entry.second.to(message);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const ErrorCode& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, ErrorCode& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* ErrorCode::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x91aec7d2ce28e5e3ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> ErrorCode::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.ErrorCode";
	type_code->type_hash = vnx::Hash64(0x91aec7d2ce28e5e3ull);
	type_code->code_hash = vnx::Hash64(0xe519ccdb44a014bdull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<ErrorCode>(); };
	type_code->fields.resize(2);
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
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::ErrorCode& value, const TypeCode* type_code, const uint16_t* code) {
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
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::ErrorCode& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::ErrorCode>(out);
		vnx::write_class_header<::vnx::web::ErrorCode>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.code);
	vnx::write(out, value.message, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::web::ErrorCode& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::ErrorCode& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::ErrorCode& value) {
	value.accept(visitor);
}

} // vnx
