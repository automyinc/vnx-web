
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/StreamWrite.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 StreamWrite::VNX_TYPE_HASH(0x6bf24232cab955eeull);
const vnx::Hash64 StreamWrite::VNX_CODE_HASH(0x28c6d1f4a773f23dull);

vnx::Hash64 StreamWrite::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* StreamWrite::get_type_name() const {
	return "vnx.web.StreamWrite";
}

std::shared_ptr<StreamWrite> StreamWrite::create() {
	return std::make_shared<StreamWrite>();
}

std::shared_ptr<vnx::Value> StreamWrite::clone() const {
	return std::make_shared<StreamWrite>(*this);
}

void StreamWrite::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void StreamWrite::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void StreamWrite::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, chunks);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, stream);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, is_eof);
	_visitor.type_end(*_type_code);
}

void StreamWrite::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"chunks\": "; vnx::write(_out, chunks);
	_out << ", \"stream\": "; vnx::write(_out, stream);
	_out << ", \"is_eof\": "; vnx::write(_out, is_eof);
	_out << "}";
}

void StreamWrite::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "chunks") {
			vnx::from_string(_entry.second, chunks);
		} else if(_entry.first == "stream") {
			vnx::from_string(_entry.second, stream);
		} else if(_entry.first == "is_eof") {
			vnx::from_string(_entry.second, is_eof);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const StreamWrite& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, StreamWrite& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* StreamWrite::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x6bf24232cab955eeull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> StreamWrite::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.StreamWrite";
	type_code->type_hash = vnx::Hash64(0x6bf24232cab955eeull);
	type_code->code_hash = vnx::Hash64(0x28c6d1f4a773f23dull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::web::BinaryData::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<StreamWrite>(); };
	type_code->fields.resize(3);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "chunks";
		field.code = {12, 12, 1};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "stream";
		field.code = {11, 2, 4};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "is_eof";
		field.code = {1};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::StreamWrite& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.is_eof, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.chunks, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.stream, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::StreamWrite& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::StreamWrite>(out);
		vnx::write_class_header<::vnx::web::StreamWrite>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(1);
	vnx::write_value(_buf + 0, value.is_eof);
	vnx::write(out, value.chunks, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.stream, type_code, type_code->fields[1].code.data());
}

void read(std::istream& in, ::vnx::web::StreamWrite& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::StreamWrite& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::StreamWrite& value) {
	value.accept(visitor);
}

} // vnx
