
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/DynamicFile.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {

const ::int32_t DynamicFile::CODE_TEXT;
const ::int32_t DynamicFile::CODE_INSERT;

const vnx::Hash64 DynamicFile::VNX_TYPE_HASH(0x2156a171fd9483b2ull);
const vnx::Hash64 DynamicFile::VNX_CODE_HASH(0xc1adbe0957c3b9ddull);

vnx::Hash64 DynamicFile::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* DynamicFile::get_type_name() const {
	return "vnx.web.DynamicFile";
}

std::shared_ptr<DynamicFile> DynamicFile::create() {
	return std::make_shared<DynamicFile>();
}

std::shared_ptr<vnx::Value> DynamicFile::clone() const {
	return std::make_shared<DynamicFile>(*this);
}

void DynamicFile::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void DynamicFile::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void DynamicFile::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, mime_type);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, time_stamp_ms);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, name);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, data);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, code);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, keys);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, chunks);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, static_context);
	_visitor.type_end(*_type_code);
}

void DynamicFile::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"mime_type\": "; vnx::write(_out, mime_type);
	_out << ", \"time_stamp_ms\": "; vnx::write(_out, time_stamp_ms);
	_out << ", \"name\": "; vnx::write(_out, name);
	_out << ", \"data\": "; vnx::write(_out, data);
	_out << ", \"code\": "; vnx::write(_out, code);
	_out << ", \"keys\": "; vnx::write(_out, keys);
	_out << ", \"chunks\": "; vnx::write(_out, chunks);
	_out << ", \"static_context\": "; vnx::write(_out, static_context);
	_out << "}";
}

void DynamicFile::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "mime_type") {
			vnx::from_string(_entry.second, mime_type);
		} else if(_entry.first == "time_stamp_ms") {
			vnx::from_string(_entry.second, time_stamp_ms);
		} else if(_entry.first == "name") {
			vnx::from_string(_entry.second, name);
		} else if(_entry.first == "data") {
			vnx::from_string(_entry.second, data);
		} else if(_entry.first == "code") {
			vnx::from_string(_entry.second, code);
		} else if(_entry.first == "keys") {
			vnx::from_string(_entry.second, keys);
		} else if(_entry.first == "chunks") {
			vnx::from_string(_entry.second, chunks);
		} else if(_entry.first == "static_context") {
			vnx::from_string(_entry.second, static_context);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const DynamicFile& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, DynamicFile& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* DynamicFile::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x2156a171fd9483b2ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> DynamicFile::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.DynamicFile";
	type_code->type_hash = vnx::Hash64(0x2156a171fd9483b2ull);
	type_code->code_hash = vnx::Hash64(0xc1adbe0957c3b9ddull);
	type_code->is_class = true;
	type_code->parents.resize(2);
	type_code->parents[0] = ::vnx::web::File::get_type_code();
	type_code->parents[1] = ::vnx::web::Content::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<DynamicFile>(); };
	type_code->fields.resize(8);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "mime_type";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "time_stamp_ms";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "name";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "data";
		field.code = {12, 1};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "code";
		field.code = {12, 7};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.is_extended = true;
		field.name = "keys";
		field.code = {12, 12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.is_extended = true;
		field.name = "chunks";
		field.code = {12, 12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "static_context";
		field.code = {24};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::DynamicFile& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.time_stamp_ms, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.mime_type, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.name, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.data, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.code, type_code, _field->code.data()); break;
			case 5: vnx::read(in, value.keys, type_code, _field->code.data()); break;
			case 6: vnx::read(in, value.chunks, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.static_context, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::DynamicFile& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::DynamicFile>(out);
		vnx::write_class_header<::vnx::web::DynamicFile>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.time_stamp_ms);
	vnx::write(out, value.mime_type, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.name, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.data, type_code, type_code->fields[3].code.data());
	vnx::write(out, value.code, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.keys, type_code, type_code->fields[5].code.data());
	vnx::write(out, value.chunks, type_code, type_code->fields[6].code.data());
	vnx::write(out, value.static_context, type_code, type_code->fields[7].code.data());
}

void read(std::istream& in, ::vnx::web::DynamicFile& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::DynamicFile& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::DynamicFile& value) {
	value.accept(visitor);
}

} // vnx
