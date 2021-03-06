
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/DynamicView.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 DynamicView::VNX_TYPE_HASH(0xa0b278abc2661c0full);
const vnx::Hash64 DynamicView::VNX_CODE_HASH(0xe0c014757ccd231ull);

vnx::Hash64 DynamicView::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* DynamicView::get_type_name() const {
	return "vnx.web.DynamicView";
}

std::shared_ptr<DynamicView> DynamicView::create() {
	return std::make_shared<DynamicView>();
}

std::shared_ptr<vnx::Value> DynamicView::clone() const {
	return std::make_shared<DynamicView>(*this);
}

void DynamicView::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void DynamicView::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void DynamicView::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, path);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, resource);
	_visitor.type_end(*_type_code);
}

void DynamicView::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"path\": "; vnx::write(_out, path);
	_out << ", \"resource\": "; vnx::write(_out, resource);
	_out << "}";
}

void DynamicView::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "path") {
			vnx::from_string(_entry.second, path);
		} else if(_entry.first == "resource") {
			vnx::from_string(_entry.second, resource);
		}
	}
}

vnx::Object DynamicView::to_object() const {
	vnx::Object _object;
	_object["path"] = path;
	_object["resource"] = resource;
	return _object;
}

void DynamicView::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "path") {
			_entry.second.to(path);
		} else if(_entry.first == "resource") {
			_entry.second.to(resource);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const DynamicView& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, DynamicView& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* DynamicView::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xa0b278abc2661c0full));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> DynamicView::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.DynamicView";
	type_code->type_hash = vnx::Hash64(0xa0b278abc2661c0full);
	type_code->code_hash = vnx::Hash64(0xe0c014757ccd231ull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::web::View::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<DynamicView>(); };
	type_code->fields.resize(2);
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
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::DynamicView& value, const TypeCode* type_code, const uint16_t* code) {
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
			case 0: vnx::read(in, value.path, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.resource, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::DynamicView& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::DynamicView>(out);
		vnx::write_class_header<::vnx::web::DynamicView>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.path, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.resource, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::web::DynamicView& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::DynamicView& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::DynamicView& value) {
	value.accept(visitor);
}

} // vnx
