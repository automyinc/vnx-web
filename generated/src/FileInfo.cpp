
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/FileInfo.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 FileInfo::VNX_TYPE_HASH(0x78c84256cbab6799ull);
const vnx::Hash64 FileInfo::VNX_CODE_HASH(0xae3dbf4f5465d29aull);

vnx::Hash64 FileInfo::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* FileInfo::get_type_name() const {
	return "vnx.web.FileInfo";
}

std::shared_ptr<FileInfo> FileInfo::create() {
	return std::make_shared<FileInfo>();
}

std::shared_ptr<vnx::Value> FileInfo::clone() const {
	return std::make_shared<FileInfo>(*this);
}

void FileInfo::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void FileInfo::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void FileInfo::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, name);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, mime_type);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, is_directory);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, num_bytes);
	_visitor.type_end(*_type_code);
}

void FileInfo::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"name\": "; vnx::write(_out, name);
	_out << ", \"mime_type\": "; vnx::write(_out, mime_type);
	_out << ", \"is_directory\": "; vnx::write(_out, is_directory);
	_out << ", \"num_bytes\": "; vnx::write(_out, num_bytes);
	_out << "}";
}

void FileInfo::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "is_directory") {
			vnx::from_string(_entry.second, is_directory);
		} else if(_entry.first == "mime_type") {
			vnx::from_string(_entry.second, mime_type);
		} else if(_entry.first == "name") {
			vnx::from_string(_entry.second, name);
		} else if(_entry.first == "num_bytes") {
			vnx::from_string(_entry.second, num_bytes);
		}
	}
}

vnx::Object FileInfo::to_object() const {
	vnx::Object _object;
	_object["name"] = name;
	_object["mime_type"] = mime_type;
	_object["is_directory"] = is_directory;
	_object["num_bytes"] = num_bytes;
	return _object;
}

void FileInfo::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "is_directory") {
			_entry.second.to(is_directory);
		} else if(_entry.first == "mime_type") {
			_entry.second.to(mime_type);
		} else if(_entry.first == "name") {
			_entry.second.to(name);
		} else if(_entry.first == "num_bytes") {
			_entry.second.to(num_bytes);
		}
	}
}

/// \private
std::ostream& operator<<(std::ostream& _out, const FileInfo& _value) {
	_value.write(_out);
	return _out;
}

/// \private
std::istream& operator>>(std::istream& _in, FileInfo& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* FileInfo::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x78c84256cbab6799ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> FileInfo::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.FileInfo";
	type_code->type_hash = vnx::Hash64(0x78c84256cbab6799ull);
	type_code->code_hash = vnx::Hash64(0xae3dbf4f5465d29aull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<FileInfo>(); };
	type_code->fields.resize(4);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "name";
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
		field.name = "is_directory";
		field.code = {1};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "num_bytes";
		field.value = vnx::to_string(-1);
		field.code = {8};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::FileInfo& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.is_directory, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[3];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.num_bytes, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.name, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.mime_type, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::FileInfo& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::FileInfo>(out);
		vnx::write_class_header<::vnx::web::FileInfo>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(9);
	vnx::write_value(_buf + 0, value.is_directory);
	vnx::write_value(_buf + 1, value.num_bytes);
	vnx::write(out, value.name, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.mime_type, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::web::FileInfo& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::FileInfo& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::FileInfo& value) {
	value.accept(visitor);
}

} // vnx
