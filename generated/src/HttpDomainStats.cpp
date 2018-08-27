
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/HttpDomainStats.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {


const vnx::Hash64 HttpDomainStats::VNX_TYPE_HASH(0x992d5238380ae57cull);
const vnx::Hash64 HttpDomainStats::VNX_CODE_HASH(0x6317d28f58b388faull);

vnx::Hash64 HttpDomainStats::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* HttpDomainStats::get_type_name() const {
	return "vnx.web.HttpDomainStats";
}

std::shared_ptr<HttpDomainStats> HttpDomainStats::create() {
	return std::make_shared<HttpDomainStats>();
}

std::shared_ptr<vnx::Value> HttpDomainStats::clone() const {
	return std::make_shared<HttpDomainStats>(*this);
}

void HttpDomainStats::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void HttpDomainStats::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void HttpDomainStats::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, error_count);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, page_count);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, referral_count);
	_visitor.type_end(*_type_code);
}

void HttpDomainStats::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"error_count\": "; vnx::write(_out, error_count);
	_out << ", \"page_count\": "; vnx::write(_out, page_count);
	_out << ", \"referral_count\": "; vnx::write(_out, referral_count);
	_out << "}";
}

void HttpDomainStats::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "error_count") {
			vnx::from_string(_entry.second, error_count);
		} else if(_entry.first == "page_count") {
			vnx::from_string(_entry.second, page_count);
		} else if(_entry.first == "referral_count") {
			vnx::from_string(_entry.second, referral_count);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const HttpDomainStats& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, HttpDomainStats& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* HttpDomainStats::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x992d5238380ae57cull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> HttpDomainStats::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.HttpDomainStats";
	type_code->type_hash = vnx::Hash64(0x992d5238380ae57cull);
	type_code->code_hash = vnx::Hash64(0x6317d28f58b388faull);
	type_code->is_class = true;
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<HttpDomainStats>(); };
	type_code->fields.resize(3);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "error_count";
		field.code = {13, 3, 7, 8};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "page_count";
		field.code = {13, 4, 12, 5, 8};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "referral_count";
		field.code = {13, 4, 12, 5, 8};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::HttpDomainStats& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] != CODE_STRUCT)) {
		vnx::skip(in, type_code, code);
		return;
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	const char* const _buf = in.read(type_code->total_field_size);
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.error_count, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.page_count, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.referral_count, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::HttpDomainStats& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::HttpDomainStats>(out);
		vnx::write_class_header<::vnx::web::HttpDomainStats>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	vnx::write(out, value.error_count, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.page_count, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.referral_count, type_code, type_code->fields[2].code.data());
}

void read(std::istream& in, ::vnx::web::HttpDomainStats& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::HttpDomainStats& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::HttpDomainStats& value) {
	value.accept(visitor);
}

} // vnx
