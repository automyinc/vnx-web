
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/Notification.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {

const ::int32_t Notification::CONTENT_NEW;
const ::int32_t Notification::CONTENT_UPDATE;
const ::int32_t Notification::CONTENT_DELETE;

const vnx::Hash64 Notification::VNX_TYPE_HASH(0xdb0cc3f801c5d452ull);
const vnx::Hash64 Notification::VNX_CODE_HASH(0x8b31c32b91c9fa57ull);

vnx::Hash64 Notification::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* Notification::get_type_name() const {
	return "vnx.web.Notification";
}

std::shared_ptr<Notification> Notification::create() {
	return std::make_shared<Notification>();
}

std::shared_ptr<vnx::Value> Notification::clone() const {
	return std::make_shared<Notification>(*this);
}

void Notification::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void Notification::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void Notification::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, path);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, type);
	_visitor.type_end(*_type_code);
}

void Notification::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"path\": "; vnx::write(_out, path);
	_out << ", \"type\": "; vnx::write(_out, type);
	_out << "}";
}

void Notification::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "path") {
			vnx::from_string(_entry.second, path);
		} else if(_entry.first == "type") {
			vnx::from_string(_entry.second, type);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const Notification& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, Notification& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* Notification::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xdb0cc3f801c5d452ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> Notification::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.Notification";
	type_code->type_hash = vnx::Hash64(0xdb0cc3f801c5d452ull);
	type_code->code_hash = vnx::Hash64(0x8b31c32b91c9fa57ull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<Notification>(); };
	type_code->fields.resize(2);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "path";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "type";
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::Notification& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] != CODE_STRUCT)) {
		vnx::skip(in, type_code, code);
		return;
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	const char* const _buf = in.read(type_code->total_field_size);
	{
		const vnx::TypeField* const _field = type_code->field_map[1];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.type, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.path, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::Notification& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::Notification>(out);
		vnx::write_class_header<::vnx::web::Notification>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(4);
	vnx::write_value(_buf + 0, value.type);
	vnx::write(out, value.path, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::web::Notification& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::Notification& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::Notification& value) {
	value.accept(visitor);
}

} // vnx
