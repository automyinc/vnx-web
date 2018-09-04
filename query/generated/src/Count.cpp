
// AUTO GENERATED by vnxcppcodegen

#include <vnx/query/package.hxx>
#include <vnx/query/Count.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace query {


const vnx::Hash64 Count::VNX_TYPE_HASH(0xcf067f1e523c9400ull);
const vnx::Hash64 Count::VNX_CODE_HASH(0x3a79362c3e5b12aeull);

vnx::Hash64 Count::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* Count::get_type_name() const {
	return "vnx.query.Count";
}

std::shared_ptr<Count> Count::create() {
	return std::make_shared<Count>();
}

std::shared_ptr<vnx::Value> Count::clone() const {
	return std::make_shared<Count>(*this);
}

void Count::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Count::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Count::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, result);
	_visitor.type_end(*_type_code);
}

void Count::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"result\": "; vnx::write(_out, result);
	_out << "}";
}

void Count::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "result") {
			vnx::from_string(_entry.second, result);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const Count& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, Count& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Count::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xcf067f1e523c9400ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Count::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.query.Count";
	type_code->type_hash = vnx::Hash64(0xcf067f1e523c9400ull);
	type_code->code_hash = vnx::Hash64(0x3a79362c3e5b12aeull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::query::Aggregate::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Count>(); };
	type_code->fields.resize(1);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.name = "result";
		field.value = vnx::to_string(0);
		field.code = {4};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace query


namespace vnx {

void read(TypeInput& in, ::vnx::query::Count& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.result, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::query::Count& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::query::Count>(out);
		vnx::write_class_header<::vnx::query::Count>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.result);
}

void read(std::istream& in, ::vnx::query::Count& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::query::Count& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::query::Count& value) {
	value.accept(visitor);
}

} // vnx
