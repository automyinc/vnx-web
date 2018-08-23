
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/CacheBase.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Struct.h>
#include <vnx/Config.h>


namespace vnx {
namespace web {


const vnx::Hash64 CacheBase::VNX_TYPE_HASH(0x6de8cc0cc0e6b5f8ull);
const vnx::Hash64 CacheBase::VNX_CODE_HASH(0xd50aa3291aae0fb2ull);

CacheBase::CacheBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".domain", domain);
	vnx::read_config(vnx_name + ".input", input);
	vnx::read_config(vnx_name + ".channel", channel);
	vnx::read_config(vnx_name + ".num_entries", num_entries);
	vnx::read_config(vnx_name + ".max_entry_size", max_entry_size);
	vnx::read_config(vnx_name + ".max_pending", max_pending);
	vnx::read_config(vnx_name + ".update_interval_ms", update_interval_ms);
	vnx::read_config(vnx_name + ".maintain_interval_ms", maintain_interval_ms);
}

vnx::Hash64 CacheBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* CacheBase::get_type_name() const {
	return "vnx.web.Cache";
}

void CacheBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, domain);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, input);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, channel);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, num_entries);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, max_entry_size);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, max_pending);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, update_interval_ms);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, maintain_interval_ms);
	_visitor.type_end(*_type_code);
}

void CacheBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"domain\": "; vnx::write(_out, domain);
	_out << ", \"input\": "; vnx::write(_out, input);
	_out << ", \"channel\": "; vnx::write(_out, channel);
	_out << ", \"num_entries\": "; vnx::write(_out, num_entries);
	_out << ", \"max_entry_size\": "; vnx::write(_out, max_entry_size);
	_out << ", \"max_pending\": "; vnx::write(_out, max_pending);
	_out << ", \"update_interval_ms\": "; vnx::write(_out, update_interval_ms);
	_out << ", \"maintain_interval_ms\": "; vnx::write(_out, maintain_interval_ms);
	_out << "}";
}

void CacheBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "domain") {
			vnx::from_string(_entry.second, domain);
		} else if(_entry.first == "input") {
			vnx::from_string(_entry.second, input);
		} else if(_entry.first == "channel") {
			vnx::from_string(_entry.second, channel);
		} else if(_entry.first == "num_entries") {
			vnx::from_string(_entry.second, num_entries);
		} else if(_entry.first == "max_entry_size") {
			vnx::from_string(_entry.second, max_entry_size);
		} else if(_entry.first == "max_pending") {
			vnx::from_string(_entry.second, max_pending);
		} else if(_entry.first == "update_interval_ms") {
			vnx::from_string(_entry.second, update_interval_ms);
		} else if(_entry.first == "maintain_interval_ms") {
			vnx::from_string(_entry.second, maintain_interval_ms);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const CacheBase& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, CacheBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* CacheBase::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x6de8cc0cc0e6b5f8ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> CacheBase::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.Cache";
	type_code->type_hash = vnx::Hash64(0x6de8cc0cc0e6b5f8ull);
	type_code->code_hash = vnx::Hash64(0xd50aa3291aae0fb2ull);
	type_code->methods.resize(5);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_Content";
		call_type->type_hash = vnx::Hash64(0x584e5d428e66fe95ull);
		call_type->code_hash = vnx::Hash64(0x72ac08c04bbd8b04ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.Cache.handle_vnx_web_Content.return";
			return_type->type_hash = vnx::Hash64(0x7aba04f7662c094cull);
			return_type->code_hash = vnx::Hash64(0x612241b480733130ull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "sample";
			field.code = {16};
		}
		call_type->build();
		type_code->methods[0] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_Provider";
		call_type->type_hash = vnx::Hash64(0xb3ca09bf0c4d5dadull);
		call_type->code_hash = vnx::Hash64(0xc79084cae733be6aull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.Cache.handle_vnx_web_Provider.return";
			return_type->type_hash = vnx::Hash64(0xc1c44065e63e5f12ull);
			return_type->code_hash = vnx::Hash64(0x620e1857458a8946ull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "sample";
			field.code = {16};
		}
		call_type->build();
		type_code->methods[1] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_Request";
		call_type->type_hash = vnx::Hash64(0x4da6e3f631b8d157ull);
		call_type->code_hash = vnx::Hash64(0x620b2d03a3e163f5ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.Cache.handle_vnx_web_Request.return";
			return_type->type_hash = vnx::Hash64(0x29d490aa1f7f1f4eull);
			return_type->code_hash = vnx::Hash64(0x139beb42b62e9b56ull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "sample";
			field.code = {16};
		}
		call_type->build();
		type_code->methods[2] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_Response";
		call_type->type_hash = vnx::Hash64(0x84c27382aa993259ull);
		call_type->code_hash = vnx::Hash64(0x214edadfb3c4453bull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.Cache.handle_vnx_web_Response.return";
			return_type->type_hash = vnx::Hash64(0xdac003fd40f67b3eull);
			return_type->code_hash = vnx::Hash64(0x5a93c2c96976fbadull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->fields.resize(1);
		{
			vnx::TypeField& field = call_type->fields[0];
			field.is_extended = true;
			field.name = "sample";
			field.code = {16};
		}
		call_type->build();
		type_code->methods[3] = vnx::register_type_code(call_type);
	}
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.purge";
		call_type->type_hash = vnx::Hash64(0x12a031b7ab016698ull);
		call_type->code_hash = vnx::Hash64(0xe7b3898eae3fc284ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.Cache.purge.return";
			return_type->type_hash = vnx::Hash64(0x6c005ad4188f3353ull);
			return_type->code_hash = vnx::Hash64(0x142599bf985ad946ull);
			return_type->is_return = true;
			return_type->build();
			call_type->return_type = vnx::register_type_code(return_type);
		}
		call_type->build();
		type_code->methods[4] = vnx::register_type_code(call_type);
	}
	type_code->fields.resize(8);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "domain";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.is_extended = true;
		field.name = "input";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.is_extended = true;
		field.name = "channel";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.name = "num_entries";
		field.value = vnx::to_string(4096);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "max_entry_size";
		field.value = vnx::to_string(4194304);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "max_pending";
		field.value = vnx::to_string(100000);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "update_interval_ms";
		field.value = vnx::to_string(500);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "maintain_interval_ms";
		field.value = vnx::to_string(5000);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}

void CacheBase::handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0x461d3a7c5b20db5full) {
		std::shared_ptr<const vnx::web::Content> _value = std::dynamic_pointer_cast<const vnx::web::Content>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x1fd4ef6be645a155ull) {
		std::shared_ptr<const vnx::web::Directory> _value = std::dynamic_pointer_cast<const vnx::web::Directory>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x91aec7d2ce28e5e3ull) {
		std::shared_ptr<const vnx::web::ErrorCode> _value = std::dynamic_pointer_cast<const vnx::web::ErrorCode>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x7e5090af37dca85dull) {
		std::shared_ptr<const vnx::web::File> _value = std::dynamic_pointer_cast<const vnx::web::File>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x84bd8b34f037a33eull) {
		std::shared_ptr<const vnx::web::HttpRequest> _value = std::dynamic_pointer_cast<const vnx::web::HttpRequest>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x5f05cae7dd441511ull) {
		std::shared_ptr<const vnx::web::HttpResponse> _value = std::dynamic_pointer_cast<const vnx::web::HttpResponse>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0xffdf72d9457e29f9ull) {
		std::shared_ptr<const vnx::web::ObjectFile> _value = std::dynamic_pointer_cast<const vnx::web::ObjectFile>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x1d2b97d959828f56ull) {
		std::shared_ptr<const vnx::web::Provider> _value = std::dynamic_pointer_cast<const vnx::web::Provider>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x53f584c8e4fef49dull) {
		std::shared_ptr<const vnx::web::Request> _value = std::dynamic_pointer_cast<const vnx::web::Request>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x2a23ede4ff56e0a2ull) {
		std::shared_ptr<const vnx::web::Response> _value = std::dynamic_pointer_cast<const vnx::web::Response>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0xf05376b178a4a968ull) {
		std::shared_ptr<const vnx::web::TextFile> _value = std::dynamic_pointer_cast<const vnx::web::TextFile>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x5be5d32e3d71e8edull) {
		std::shared_ptr<const vnx::web::ValueFile> _value = std::dynamic_pointer_cast<const vnx::web::ValueFile>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	}
}

bool CacheBase::call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type) {
	if(_call_type->type_hash == vnx::Hash64(0x584e5d428e66fe95ull)) {
		::std::shared_ptr<const ::vnx::web::Content> sample;
		const char* const _buf = _in.read(_call_type->total_field_size);
		for(const vnx::TypeField* _field : _call_type->ext_fields) {
			switch(_field->native_index) {
				case 0: vnx::read(_in, sample, _call_type, _field->code.data()); break;
				default: vnx::skip(_in, _call_type, _field->code.data());
			}
		}
		handle(sample);
		return true;
	} else if(_call_type->type_hash == vnx::Hash64(0xb3ca09bf0c4d5dadull)) {
		::std::shared_ptr<const ::vnx::web::Provider> sample;
		const char* const _buf = _in.read(_call_type->total_field_size);
		for(const vnx::TypeField* _field : _call_type->ext_fields) {
			switch(_field->native_index) {
				case 0: vnx::read(_in, sample, _call_type, _field->code.data()); break;
				default: vnx::skip(_in, _call_type, _field->code.data());
			}
		}
		handle(sample);
		return true;
	} else if(_call_type->type_hash == vnx::Hash64(0x4da6e3f631b8d157ull)) {
		::std::shared_ptr<const ::vnx::web::Request> sample;
		const char* const _buf = _in.read(_call_type->total_field_size);
		for(const vnx::TypeField* _field : _call_type->ext_fields) {
			switch(_field->native_index) {
				case 0: vnx::read(_in, sample, _call_type, _field->code.data()); break;
				default: vnx::skip(_in, _call_type, _field->code.data());
			}
		}
		handle(sample);
		return true;
	} else if(_call_type->type_hash == vnx::Hash64(0x84c27382aa993259ull)) {
		::std::shared_ptr<const ::vnx::web::Response> sample;
		const char* const _buf = _in.read(_call_type->total_field_size);
		for(const vnx::TypeField* _field : _call_type->ext_fields) {
			switch(_field->native_index) {
				case 0: vnx::read(_in, sample, _call_type, _field->code.data()); break;
				default: vnx::skip(_in, _call_type, _field->code.data());
			}
		}
		handle(sample);
		return true;
	} else if(_call_type->type_hash == vnx::Hash64(0x12a031b7ab016698ull)) {
		const char* const _buf = _in.read(_call_type->total_field_size);
		for(const vnx::TypeField* _field : _call_type->ext_fields) {
			switch(_field->native_index) {
				default: vnx::skip(_in, _call_type, _field->code.data());
			}
		}
		purge();
		return true;
	}
	return false;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::CacheBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] != CODE_STRUCT)) {
		vnx::skip(in, type_code, code);
		return;
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	const char* const _buf = in.read(type_code->total_field_size);
	{
		const vnx::TypeField* const _field = type_code->field_map[3];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.num_entries, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[4];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.max_entry_size, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[5];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.max_pending, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[6];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.update_interval_ms, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[7];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.maintain_interval_ms, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.domain, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.input, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.channel, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::CacheBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::CacheBase>(out);
		vnx::write_class_header<::vnx::web::CacheBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(32);
	vnx::write_value(_buf + 0, value.num_entries);
	vnx::write_value(_buf + 8, value.max_entry_size);
	vnx::write_value(_buf + 16, value.max_pending);
	vnx::write_value(_buf + 24, value.update_interval_ms);
	vnx::write_value(_buf + 28, value.maintain_interval_ms);
	vnx::write(out, value.domain, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.input, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.channel, type_code, type_code->fields[2].code.data());
}

void read(std::istream& in, ::vnx::web::CacheBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::CacheBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::CacheBase& value) {
	value.accept(visitor);
}

} // vnx
