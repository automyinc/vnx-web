
// AUTO GENERATED by vnxcppcodegen

#include <vnx/query/package.hxx>
#include <vnx/query/NotEqual.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace query {


const vnx::Hash64 NotEqual::VNX_TYPE_HASH(0x301039df7e0f5141ull);
const vnx::Hash64 NotEqual::VNX_CODE_HASH(0xdf2e0d1654da6ffeull);

vnx::Hash64 NotEqual::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* NotEqual::get_type_name() const {
	return "vnx.query.NotEqual";
}

std::shared_ptr<NotEqual> NotEqual::create() {
	return std::make_shared<NotEqual>();
}

std::shared_ptr<vnx::Value> NotEqual::clone() const {
	return std::make_shared<NotEqual>(*this);
}

void NotEqual::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void NotEqual::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void NotEqual::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, A);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, B);
	_visitor.type_end(*_type_code);
}

void NotEqual::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"A\": "; vnx::write(_out, A);
	_out << ", \"B\": "; vnx::write(_out, B);
	_out << "}";
}

void NotEqual::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "A") {
			vnx::from_string(_entry.second, A);
		} else if(_entry.first == "B") {
			vnx::from_string(_entry.second, B);
		}
	}
}

vnx::Object NotEqual::to_object() const {
	vnx::Object _object;
	_object["A"] = A;
	_object["B"] = B;
	return _object;
}

void NotEqual::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "A") {
			_entry.second.to(A);
		} else if(_entry.first == "B") {
			_entry.second.to(B);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const NotEqual& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, NotEqual& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* NotEqual::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x301039df7e0f5141ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> NotEqual::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.query.NotEqual";
	type_code->type_hash = vnx::Hash64(0x301039df7e0f5141ull);
	type_code->code_hash = vnx::Hash64(0xdf2e0d1654da6ffeull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::query::Expression::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<NotEqual>(); };
	type_code->fields.resize(2);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "A";
		field.code = {16};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "B";
		field.code = {16};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace query


namespace vnx {

void read(TypeInput& in, ::vnx::query::NotEqual& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.A, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.B, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::query::NotEqual& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::query::NotEqual>(out);
		vnx::write_class_header<::vnx::query::NotEqual>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.A, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.B, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::query::NotEqual& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::query::NotEqual& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::query::NotEqual& value) {
	value.accept(visitor);
}

} // vnx
