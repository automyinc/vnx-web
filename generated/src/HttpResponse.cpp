
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/HttpResponse.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 HttpResponse::VNX_TYPE_HASH(0x5f05cae7dd441511ull);
const vnx::Hash64 HttpResponse::VNX_CODE_HASH(0xc8028d6b02a29eeaull);

vnx::Hash64 HttpResponse::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* HttpResponse::get_type_name() const {
	return "vnx.web.HttpResponse";
}

std::shared_ptr<HttpResponse> HttpResponse::create() {
	return std::make_shared<HttpResponse>();
}

std::shared_ptr<vnx::Value> HttpResponse::clone() const {
	return std::make_shared<HttpResponse>(*this);
}

void HttpResponse::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void HttpResponse::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void HttpResponse::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, id);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, content);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, time_to_live_ms);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, is_dynamic);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, stream);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, sequence);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, status);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, header);
	_visitor.type_end(*_type_code);
}

void HttpResponse::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"id\": "; vnx::write(_out, id);
	_out << ", \"content\": "; vnx::write(_out, content);
	_out << ", \"time_to_live_ms\": "; vnx::write(_out, time_to_live_ms);
	_out << ", \"is_dynamic\": "; vnx::write(_out, is_dynamic);
	_out << ", \"stream\": "; vnx::write(_out, stream);
	_out << ", \"sequence\": "; vnx::write(_out, sequence);
	_out << ", \"status\": "; vnx::write(_out, status);
	_out << ", \"header\": "; vnx::write(_out, header);
	_out << "}";
}

void HttpResponse::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "id") {
			vnx::from_string(_entry.second, id);
		} else if(_entry.first == "content") {
			vnx::from_string(_entry.second, content);
		} else if(_entry.first == "time_to_live_ms") {
			vnx::from_string(_entry.second, time_to_live_ms);
		} else if(_entry.first == "is_dynamic") {
			vnx::from_string(_entry.second, is_dynamic);
		} else if(_entry.first == "stream") {
			vnx::from_string(_entry.second, stream);
		} else if(_entry.first == "sequence") {
			vnx::from_string(_entry.second, sequence);
		} else if(_entry.first == "status") {
			vnx::from_string(_entry.second, status);
		} else if(_entry.first == "header") {
			vnx::from_string(_entry.second, header);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const HttpResponse& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, HttpResponse& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* HttpResponse::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x5f05cae7dd441511ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> HttpResponse::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.HttpResponse";
	type_code->type_hash = vnx::Hash64(0x5f05cae7dd441511ull);
	type_code->code_hash = vnx::Hash64(0xc8028d6b02a29eeaull);
	type_code->is_class = true;
	type_code->parents.resize(1);
	type_code->parents[0] = ::vnx::web::Response::get_type_code();
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<HttpResponse>(); };
	type_code->fields.resize(8);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "id";
		field.code = {11, 2, 4};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "content";
		field.code = {16};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "time_to_live_ms";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "is_dynamic";
		field.value = vnx::to_string(true);
		field.code = {1};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.is_extended = true;
		field.name = "stream";
		field.code = {11, 2, 4};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "sequence";
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "status";
		field.value = vnx::to_string(500);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.is_extended = true;
		field.name = "header";
		field.code = {12, 23, 2, 4, 6, 12, 5, 12, 5};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::HttpResponse& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.time_to_live_ms, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[3];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.is_dynamic, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[5];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.sequence, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[6];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.status, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.id, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.content, type_code, _field->code.data()); break;
			case 4: vnx::read(in, value.stream, type_code, _field->code.data()); break;
			case 7: vnx::read(in, value.header, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::HttpResponse& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::HttpResponse>(out);
		vnx::write_class_header<::vnx::web::HttpResponse>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(21);
	vnx::write_value(_buf + 0, value.time_to_live_ms);
	vnx::write_value(_buf + 8, value.is_dynamic);
	vnx::write_value(_buf + 9, value.sequence);
	vnx::write_value(_buf + 17, value.status);
	vnx::write(out, value.id, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.content, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.stream, type_code, type_code->fields[4].code.data());
	vnx::write(out, value.header, type_code, type_code->fields[7].code.data());
}

void read(std::istream& in, ::vnx::web::HttpResponse& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::HttpResponse& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::HttpResponse& value) {
	value.accept(visitor);
}

} // vnx
