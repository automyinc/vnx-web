
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/File.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 File::VNX_TYPE_HASH(0x7e5090af37dca85dull);
const vnx::Hash64 File::VNX_CODE_HASH(0xdfe5356f9fbe2f43ull);

vnx::Hash64 File::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* File::get_type_name() const {
	return "vnx.web.File";
}

std::shared_ptr<File> File::create() {
	return std::make_shared<File>();
}

std::shared_ptr<vnx::Value> File::clone() const {
	return std::make_shared<File>(*this);
}

void File::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void File::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void File::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, path);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, mime_type);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, time_stamp_ms);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, data);
	_visitor.type_end(*_type_code);
}

void File::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"path\": "; vnx::write(_out, path);
	_out << ", \"mime_type\": "; vnx::write(_out, mime_type);
	_out << ", \"time_stamp_ms\": "; vnx::write(_out, time_stamp_ms);
	_out << ", \"data\": "; vnx::write(_out, data);
	_out << "}";
}

void File::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "path") {
			vnx::from_string(_entry.second, path);
		} else if(_entry.first == "mime_type") {
			vnx::from_string(_entry.second, mime_type);
		} else if(_entry.first == "time_stamp_ms") {
			vnx::from_string(_entry.second, time_stamp_ms);
		} else if(_entry.first == "data") {
			vnx::from_string(_entry.second, data);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const File& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, File& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* File::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x7e5090af37dca85dull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> File::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.File";
	type_code->type_hash = vnx::Hash64(0x7e5090af37dca85dull);
	type_code->code_hash = vnx::Hash64(0xdfe5356f9fbe2f43ull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::web::Content::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<File>(); };
	type_code->fields.resize(4);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "path";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "mime_type";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "time_stamp_ms";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "data";
		field.code = {12, 1};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::File& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] != CODE_STRUCT)) {
		vnx::skip(in, type_code, code);
		return;
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	const char* const _buf = in.read(type_code->total_field_size);
	{
		const vnx::TypeField* const _field = type_code->field_map[2];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.time_stamp_ms, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.path, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.mime_type, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.data, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::File& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::File>(out);
		vnx::write_class_header<::vnx::web::File>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.time_stamp_ms);
	vnx::write(out, value.path, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.mime_type, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.data, type_code, type_code->fields[3].code.data());
}

void read(std::istream& in, ::vnx::web::File& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::File& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::File& value) {
	value.accept(visitor);
}

} // vnx
