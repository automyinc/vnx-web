
// AUTO GENERATED by vnxcppcodegen

#include <vnx/query/package.hxx>
#include <vnx/query/Min.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace query {


const vnx::Hash64 Min::VNX_TYPE_HASH(0xf5c5d495ee64b93eull);
const vnx::Hash64 Min::VNX_CODE_HASH(0x3f43a2a7c9dd140full);

vnx::Hash64 Min::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* Min::get_type_name() const {
	return "vnx.query.Min";
}

std::shared_ptr<Min> Min::create() {
	return std::make_shared<Min>();
}

std::shared_ptr<vnx::Value> Min::clone() const {
	return std::make_shared<Min>(*this);
}

void Min::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Min::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Min::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, A);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, result);
	_visitor.type_end(*_type_code);
}

void Min::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"A\": "; vnx::write(_out, A);
	_out << ", \"result\": "; vnx::write(_out, result);
	_out << "}";
}

void Min::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "A") {
			vnx::from_string(_entry.second, A);
		} else if(_entry.first == "result") {
			vnx::from_string(_entry.second, result);
		}
	}
}

vnx::Object Min::to_object() const {
	vnx::Object _object;
	_object["A"] = A;
	_object["result"] = result;
	return _object;
}

void Min::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "A") {
			_entry.second.to(A);
		} else if(_entry.first == "result") {
			_entry.second.to(result);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const Min& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, Min& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Min::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xf5c5d495ee64b93eull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Min::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.query.Min";
	type_code->type_hash = vnx::Hash64(0xf5c5d495ee64b93eull);
	type_code->code_hash = vnx::Hash64(0x3f43a2a7c9dd140full);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::query::Aggregate::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Min>(); };
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
		field.name = "result";
		field.code = {17};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace query


namespace vnx {

void read(TypeInput& in, ::vnx::query::Min& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 1: vnx::read(in, value.result, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::query::Min& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::query::Min>(out);
		vnx::write_class_header<::vnx::query::Min>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.A, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.result, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::query::Min& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::query::Min& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::query::Min& value) {
	value.accept(visitor);
}

} // vnx
