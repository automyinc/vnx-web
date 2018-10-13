
// AUTO GENERATED by vnxcppcodegen

#include <vnx/query/package.hxx>
#include <vnx/query/OrderBy.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace query {

const ::int32_t OrderBy::ASC;
const ::int32_t OrderBy::DESC;

const vnx::Hash64 OrderBy::VNX_TYPE_HASH(0xb9dc0295d667e70cull);
const vnx::Hash64 OrderBy::VNX_CODE_HASH(0xd7c2fa39f6f79df9ull);

vnx::Hash64 OrderBy::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* OrderBy::get_type_name() const {
	return "vnx.query.OrderBy";
}

std::shared_ptr<OrderBy> OrderBy::create() {
	return std::make_shared<OrderBy>();
}

std::shared_ptr<vnx::Value> OrderBy::clone() const {
	return std::make_shared<OrderBy>(*this);
}

void OrderBy::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void OrderBy::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void OrderBy::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, field);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, mode);
	_visitor.type_end(*_type_code);
}

void OrderBy::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"field\": "; vnx::write(_out, field);
	_out << ", \"mode\": "; vnx::write(_out, mode);
	_out << "}";
}

void OrderBy::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "field") {
			vnx::from_string(_entry.second, field);
		} else if(_entry.first == "mode") {
			vnx::from_string(_entry.second, mode);
		}
	}
}

vnx::Object OrderBy::to_object() const {
	vnx::Object _object;
	_object["field"] = field;
	_object["mode"] = mode;
	return _object;
}

void OrderBy::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "field") {
			_entry.second.to(field);
		} else if(_entry.first == "mode") {
			_entry.second.to(mode);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const OrderBy& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, OrderBy& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* OrderBy::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xb9dc0295d667e70cull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> OrderBy::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.query.OrderBy";
	type_code->type_hash = vnx::Hash64(0xb9dc0295d667e70cull);
	type_code->code_hash = vnx::Hash64(0xd7c2fa39f6f79df9ull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<OrderBy>(); };
	type_code->fields.resize(2);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "field";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "mode";
		field.value = vnx::to_string(1);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace query


namespace vnx {

void read(TypeInput& in, ::vnx::query::OrderBy& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.mode, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.field, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::query::OrderBy& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::query::OrderBy>(out);
		vnx::write_class_header<::vnx::query::OrderBy>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.mode);
	vnx::write(out, value.field, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::query::OrderBy& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::query::OrderBy& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::query::OrderBy& value) {
	value.accept(visitor);
}

} // vnx
