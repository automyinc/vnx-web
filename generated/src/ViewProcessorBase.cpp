
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/ViewProcessorBase.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>
#include <vnx/Config.h>


namespace vnx {
namespace web {


const vnx::Hash64 ViewProcessorBase::VNX_TYPE_HASH(0x7197a577b12bad0bull);
const vnx::Hash64 ViewProcessorBase::VNX_CODE_HASH(0xaa1dd60280850f3aull);

ViewProcessorBase::ViewProcessorBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".channel", channel);
	vnx::read_config(vnx_name + ".domain", domain);
	vnx::read_config(vnx_name + ".input", input);
	vnx::read_config(vnx_name + ".max_input_queue_ms", max_input_queue_ms);
	vnx::read_config(vnx_name + ".output", output);
	vnx::read_config(vnx_name + ".render_interval_ms", render_interval_ms);
	vnx::read_config(vnx_name + ".update_interval_ms", update_interval_ms);
}

vnx::Hash64 ViewProcessorBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* ViewProcessorBase::get_type_name() const {
	return "vnx.web.ViewProcessor";
}

void ViewProcessorBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, domain);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, input);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, channel);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, output);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, max_input_queue_ms);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, render_interval_ms);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, update_interval_ms);
	_visitor.type_end(*_type_code);
}

void ViewProcessorBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"domain\": "; vnx::write(_out, domain);
	_out << ", \"input\": "; vnx::write(_out, input);
	_out << ", \"channel\": "; vnx::write(_out, channel);
	_out << ", \"output\": "; vnx::write(_out, output);
	_out << ", \"max_input_queue_ms\": "; vnx::write(_out, max_input_queue_ms);
	_out << ", \"render_interval_ms\": "; vnx::write(_out, render_interval_ms);
	_out << ", \"update_interval_ms\": "; vnx::write(_out, update_interval_ms);
	_out << "}";
}

void ViewProcessorBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "channel") {
			vnx::from_string(_entry.second, channel);
		} else if(_entry.first == "domain") {
			vnx::from_string(_entry.second, domain);
		} else if(_entry.first == "input") {
			vnx::from_string(_entry.second, input);
		} else if(_entry.first == "max_input_queue_ms") {
			vnx::from_string(_entry.second, max_input_queue_ms);
		} else if(_entry.first == "output") {
			vnx::from_string(_entry.second, output);
		} else if(_entry.first == "render_interval_ms") {
			vnx::from_string(_entry.second, render_interval_ms);
		} else if(_entry.first == "update_interval_ms") {
			vnx::from_string(_entry.second, update_interval_ms);
		}
	}
}

vnx::Object ViewProcessorBase::to_object() const {
	vnx::Object _object;
	_object["domain"] = domain;
	_object["input"] = input;
	_object["channel"] = channel;
	_object["output"] = output;
	_object["max_input_queue_ms"] = max_input_queue_ms;
	_object["render_interval_ms"] = render_interval_ms;
	_object["update_interval_ms"] = update_interval_ms;
	return _object;
}

void ViewProcessorBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "channel") {
			_entry.second.to(channel);
		} else if(_entry.first == "domain") {
			_entry.second.to(domain);
		} else if(_entry.first == "input") {
			_entry.second.to(input);
		} else if(_entry.first == "max_input_queue_ms") {
			_entry.second.to(max_input_queue_ms);
		} else if(_entry.first == "output") {
			_entry.second.to(output);
		} else if(_entry.first == "render_interval_ms") {
			_entry.second.to(render_interval_ms);
		} else if(_entry.first == "update_interval_ms") {
			_entry.second.to(update_interval_ms);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const ViewProcessorBase& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, ViewProcessorBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* ViewProcessorBase::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0x7197a577b12bad0bull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> ViewProcessorBase::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.ViewProcessor";
	type_code->type_hash = vnx::Hash64(0x7197a577b12bad0bull);
	type_code->code_hash = vnx::Hash64(0xaa1dd60280850f3aull);
	type_code->methods.resize(2);
	{
		std::shared_ptr<vnx::TypeCode> call_type = std::make_shared<vnx::TypeCode>(true);
		call_type->name = "vnx.web.handle_vnx_web_Request";
		call_type->type_hash = vnx::Hash64(0x4da6e3f631b8d157ull);
		call_type->code_hash = vnx::Hash64(0x13c91ed2017243c2ull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.ViewProcessor.handle_vnx_web_Request.return";
			return_type->type_hash = vnx::Hash64(0x3d112efaf649eebull);
			return_type->code_hash = vnx::Hash64(0xc0d7d942523704abull);
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
		call_type->name = "vnx.web.handle_vnx_web_Response";
		call_type->type_hash = vnx::Hash64(0x84c27382aa993259ull);
		call_type->code_hash = vnx::Hash64(0x508ce90e1157650cull);
		call_type->is_method = true;
		{
			std::shared_ptr<vnx::TypeCode> return_type = std::make_shared<vnx::TypeCode>(true);
			return_type->name = "vnx.web.ViewProcessor.handle_vnx_web_Response.return";
			return_type->type_hash = vnx::Hash64(0x204f9ca08c8fa233ull);
			return_type->code_hash = vnx::Hash64(0x4e6919dfa5b57d30ull);
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
	type_code->fields.resize(7);
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
		field.is_extended = true;
		field.name = "output";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "max_input_queue_ms";
		field.value = vnx::to_string(100);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "render_interval_ms";
		field.value = vnx::to_string(3000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "update_interval_ms";
		field.value = vnx::to_string(500);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}

void ViewProcessorBase::handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
	if(_type_hash == 0x84bd8b34f037a33eull) {
		std::shared_ptr<const vnx::web::HttpRequest> _value = std::dynamic_pointer_cast<const vnx::web::HttpRequest>(_sample->value);
		if(_value) {
			handle(_value, _sample);
		}
	} else if(_type_hash == 0x5f05cae7dd441511ull) {
		std::shared_ptr<const vnx::web::HttpResponse> _value = std::dynamic_pointer_cast<const vnx::web::HttpResponse>(_sample->value);
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
	}
}

bool ViewProcessorBase::call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type) {
	if(_call_type->type_hash == vnx::Hash64(0x4da6e3f631b8d157ull)) {
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
	}
	return false;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::ViewProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
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
		const vnx::TypeField* const _field = type_code->field_map[4];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.max_input_queue_ms, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[5];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.render_interval_ms, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[6];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.update_interval_ms, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.domain, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.input, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.channel, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.output, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::ViewProcessorBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::ViewProcessorBase>(out);
		vnx::write_class_header<::vnx::web::ViewProcessorBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(12);
	vnx::write_value(_buf + 0, value.max_input_queue_ms);
	vnx::write_value(_buf + 4, value.render_interval_ms);
	vnx::write_value(_buf + 8, value.update_interval_ms);
	vnx::write(out, value.domain, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.input, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.channel, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.output, type_code, type_code->fields[3].code.data());
}

void read(std::istream& in, ::vnx::web::ViewProcessorBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::ViewProcessorBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::ViewProcessorBase& value) {
	value.accept(visitor);
}

} // vnx
