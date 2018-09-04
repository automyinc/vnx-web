
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/DefaultView.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 DefaultView::VNX_TYPE_HASH(0xc0b79b7fe736a1f9ull);
const vnx::Hash64 DefaultView::VNX_CODE_HASH(0xbe824f1b8e2a23d6ull);

vnx::Hash64 DefaultView::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* DefaultView::get_type_name() const {
	return "vnx.web.DefaultView";
}

std::shared_ptr<DefaultView> DefaultView::create() {
	return std::make_shared<DefaultView>();
}

std::shared_ptr<vnx::Value> DefaultView::clone() const {
	return std::make_shared<DefaultView>(*this);
}

void DefaultView::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void DefaultView::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void DefaultView::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, path);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, resource);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, allow_directory_index);
	_visitor.type_end(*_type_code);
}

void DefaultView::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"path\": "; vnx::write(_out, path);
	_out << ", \"resource\": "; vnx::write(_out, resource);
	_out << ", \"allow_directory_index\": "; vnx::write(_out, allow_directory_index);
	_out << "}";
}

void DefaultView::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "path") {
			vnx::from_string(_entry.second, path);
		} else if(_entry.first == "resource") {
			vnx::from_string(_entry.second, resource);
		} else if(_entry.first == "allow_directory_index") {
			vnx::from_string(_entry.second, allow_directory_index);
		}
	}
}

vnx::Object DefaultView::to_object() const {
	vnx::Object _object;
	_object["path"] = path;
	_object["resource"] = resource;
	_object["allow_directory_index"] = allow_directory_index;
	return _object;
}

void DefaultView::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "path") {
			_entry.second.to(path);
		} else if(_entry.first == "resource") {
			_entry.second.to(resource);
		} else if(_entry.first == "allow_directory_index") {
			_entry.second.to(allow_directory_index);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const DefaultView& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, DefaultView& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* DefaultView::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xc0b79b7fe736a1f9ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> DefaultView::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.DefaultView";
	type_code->type_hash = vnx::Hash64(0xc0b79b7fe736a1f9ull);
	type_code->code_hash = vnx::Hash64(0xbe824f1b8e2a23d6ull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::web::View::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<DefaultView>(); };
	type_code->fields.resize(3);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "path";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "resource";
		field.code = {13, 4, 12, 5, 16};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "allow_directory_index";
		field.value = vnx::to_string(true);
		field.code = {1};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::DefaultView& value, const TypeCode* type_code, const uint16_t* code) {
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
		const vnx::TypeField* const _field = type_code->field_map[2];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.allow_directory_index, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.path, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.resource, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::DefaultView& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::DefaultView>(out);
		vnx::write_class_header<::vnx::web::DefaultView>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(1);
	vnx::write_value(_buf + 0, value.allow_directory_index);
	vnx::write(out, value.path, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.resource, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::web::DefaultView& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::DefaultView& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::DefaultView& value) {
	value.accept(visitor);
}

} // vnx
