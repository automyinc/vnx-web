
// AUTO GENERATED by vnxcppcodegen

#include <vnx/query/package.hxx>
#include <vnx/query/Select.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace query {


const vnx::Hash64 Select::VNX_TYPE_HASH(0xb2ec0d2037f0e3a2ull);
const vnx::Hash64 Select::VNX_CODE_HASH(0x19645b73e9eb7f88ull);

vnx::Hash64 Select::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* Select::get_type_name() const {
	return "vnx.query.Select";
}

std::shared_ptr<Select> Select::create() {
	return std::make_shared<Select>();
}

std::shared_ptr<vnx::Value> Select::clone() const {
	return std::make_shared<Select>(*this);
}

void Select::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Select::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Select::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, fields);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, aggregates);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, from);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, where);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, group_by);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, having);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, order_by);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, limit);
	_visitor.type_end(*_type_code);
}

void Select::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"fields\": "; vnx::write(_out, fields);
	_out << ", \"aggregates\": "; vnx::write(_out, aggregates);
	_out << ", \"from\": "; vnx::write(_out, from);
	_out << ", \"where\": "; vnx::write(_out, where);
	_out << ", \"group_by\": "; vnx::write(_out, group_by);
	_out << ", \"having\": "; vnx::write(_out, having);
	_out << ", \"order_by\": "; vnx::write(_out, order_by);
	_out << ", \"limit\": "; vnx::write(_out, limit);
	_out << "}";
}

void Select::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "fields") {
			vnx::from_string(_entry.second, fields);
		} else if(_entry.first == "aggregates") {
			vnx::from_string(_entry.second, aggregates);
		} else if(_entry.first == "from") {
			vnx::from_string(_entry.second, from);
		} else if(_entry.first == "where") {
			vnx::from_string(_entry.second, where);
		} else if(_entry.first == "group_by") {
			vnx::from_string(_entry.second, group_by);
		} else if(_entry.first == "having") {
			vnx::from_string(_entry.second, having);
		} else if(_entry.first == "order_by") {
			vnx::from_string(_entry.second, order_by);
		} else if(_entry.first == "limit") {
			vnx::from_string(_entry.second, limit);
		}
	}
}

vnx::Object Select::to_object() const {
	vnx::Object _object;
	_object["fields"] = fields;
	_object["aggregates"] = aggregates;
	_object["from"] = from;
	_object["where"] = where;
	_object["group_by"] = group_by;
	_object["having"] = having;
	_object["order_by"] = order_by;
	_object["limit"] = limit;
	return _object;
}

void Select::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "fields") {
			_entry.second.to(fields);
		} else if(_entry.first == "aggregates") {
			_entry.second.to(aggregates);
		} else if(_entry.first == "from") {
			_entry.second.to(from);
		} else if(_entry.first == "where") {
			_entry.second.to(where);
		} else if(_entry.first == "group_by") {
			_entry.second.to(group_by);
		} else if(_entry.first == "having") {
			_entry.second.to(having);
		} else if(_entry.first == "order_by") {
			_entry.second.to(order_by);
		} else if(_entry.first == "limit") {
			_entry.second.to(limit);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const Select& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, Select& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Select::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xb2ec0d2037f0e3a2ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Select::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.query.Select";
	type_code->type_hash = vnx::Hash64(0xb2ec0d2037f0e3a2ull);
	type_code->code_hash = vnx::Hash64(0x19645b73e9eb7f88ull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::query::Query::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Select>(); };
	type_code->fields.resize(8);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "fields";
		field.code = {13, 4, 12, 5, 16};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "aggregates";
		field.code = {13, 4, 12, 5, 16};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "from";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "where";
		field.code = {16};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "group_by";
		field.code = {16};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "having";
		field.code = {16};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.is_extended = true;
		field.name = "order_by";
		field.code = {16};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "limit";
		field.code = {16};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace query


namespace vnx {

void read(TypeInput& in, ::vnx::query::Select& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.fields, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.aggregates, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.from, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.where, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.group_by, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.having, type_code, _field->code.data()); break;
			case 6: vnx::read(in, value.order_by, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.limit, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::query::Select& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::query::Select>(out);
		vnx::write_class_header<::vnx::query::Select>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.fields, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.aggregates, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.from, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.where, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.group_by, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.having, type_code, type_code->fields[5].code.data());
	vnx::write(out, value.order_by, type_code, type_code->fields[6].code.data());
	vnx::write(out, value.limit, type_code, type_code->fields[7].code.data());
}

void read(std::istream& in, ::vnx::query::Select& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::query::Select& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::query::Select& value) {
	value.accept(visitor);
}

} // vnx
