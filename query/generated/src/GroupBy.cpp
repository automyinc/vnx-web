
// AUTO GENERATED by vnxcppcodegen

#include <vnx/query/package.hxx>
#include <vnx/query/GroupBy.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace query {


const vnx::Hash64 GroupBy::VNX_TYPE_HASH(0xbc2edcf64acd47a7ull);
const vnx::Hash64 GroupBy::VNX_CODE_HASH(0x56f7d2a0eb68026cull);

vnx::Hash64 GroupBy::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* GroupBy::get_type_name() const {
	return "vnx.query.GroupBy";
}

std::shared_ptr<GroupBy> GroupBy::create() {
	return std::make_shared<GroupBy>();
}

std::shared_ptr<vnx::Value> GroupBy::clone() const {
	return std::make_shared<GroupBy>(*this);
}

void GroupBy::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void GroupBy::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void GroupBy::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, field);
	_visitor.type_end(*_type_code);
}

void GroupBy::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"field\": "; vnx::write(_out, field);
	_out << "}";
}

void GroupBy::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "field") {
			vnx::from_string(_entry.second, field);
		}
	}
}

vnx::Object GroupBy::to_object() const {
	vnx::Object _object;
	_object["field"] = field;
	return _object;
}

void GroupBy::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "field") {
			_entry.second.to(field);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const GroupBy& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, GroupBy& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* GroupBy::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xbc2edcf64acd47a7ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> GroupBy::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.query.GroupBy";
	type_code->type_hash = vnx::Hash64(0xbc2edcf64acd47a7ull);
	type_code->code_hash = vnx::Hash64(0x56f7d2a0eb68026cull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<GroupBy>(); };
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "field";
		field.code = {12, 5};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace query


namespace vnx {

void read(TypeInput& in, ::vnx::query::GroupBy& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.field, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::query::GroupBy& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::query::GroupBy>(out);
		vnx::write_class_header<::vnx::query::GroupBy>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.field, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::query::GroupBy& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::query::GroupBy& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::query::GroupBy& value) {
	value.accept(visitor);
}

} // vnx
