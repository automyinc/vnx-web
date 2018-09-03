
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 Provider::VNX_TYPE_HASH(0x1d2b97d959828f56ull);
const vnx::Hash64 Provider::VNX_CODE_HASH(0xae1e76ea9bf5fbf3ull);

vnx::Hash64 Provider::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* Provider::get_type_name() const {
	return "vnx.web.Provider";
}

std::shared_ptr<Provider> Provider::create() {
	return std::make_shared<Provider>();
}

std::shared_ptr<vnx::Value> Provider::clone() const {
	return std::make_shared<Provider>(*this);
}

void Provider::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Provider::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Provider::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, id);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, path);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, input);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, level);
	_visitor.type_end(*_type_code);
}

void Provider::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"id\": "; vnx::write(_out, id);
	_out << ", \"path\": "; vnx::write(_out, path);
	_out << ", \"input\": "; vnx::write(_out, input);
	_out << ", \"level\": "; vnx::write(_out, level);
	_out << "}";
}

void Provider::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "id") {
			vnx::from_string(_entry.second, id);
		} else if(_entry.first == "path") {
			vnx::from_string(_entry.second, path);
		} else if(_entry.first == "input") {
			vnx::from_string(_entry.second, input);
		} else if(_entry.first == "level") {
			vnx::from_string(_entry.second, level);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const Provider& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, Provider& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Provider::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x1d2b97d959828f56ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Provider::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.Provider";
	type_code->type_hash = vnx::Hash64(0x1d2b97d959828f56ull);
	type_code->code_hash = vnx::Hash64(0xae1e76ea9bf5fbf3ull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Provider>(); };
	type_code->fields.resize(4);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "id";
		field.code = {11, 2, 4};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "path";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "input";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "level";
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::Provider& value, const TypeCode* type_code, const uint16_t* code) {
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
		const vnx::TypeField* const _field = type_code->field_map[3];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.level, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.id, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.path, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.input, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::Provider& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::Provider>(out);
		vnx::write_class_header<::vnx::web::Provider>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.level);
	vnx::write(out, value.id, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.path, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.input, type_code, type_code->fields[2].code.data());
}

void read(std::istream& in, ::vnx::web::Provider& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::Provider& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::Provider& value) {
	value.accept(visitor);
}

} // vnx
