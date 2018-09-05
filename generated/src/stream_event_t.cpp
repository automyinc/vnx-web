
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/stream_event_t.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>


namespace vnx {
namespace web {

const ::int32_t stream_event_t::EVENT_NEW;
const ::int32_t stream_event_t::EVENT_CLOSE;
const ::int32_t stream_event_t::EVENT_EOF;

const vnx::Hash64 stream_event_t::VNX_TYPE_HASH(0xd4f7a89285ca745aull);
const vnx::Hash64 stream_event_t::VNX_CODE_HASH(0x471264f930a696f9ull);

vnx::Hash64 stream_event_t::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* stream_event_t::get_type_name() const {
	return "vnx.web.stream_event_t";
}

std::shared_ptr<stream_event_t> stream_event_t::create() {
	return std::make_shared<stream_event_t>();
}

std::shared_ptr<stream_event_t> stream_event_t::clone() const {
	return std::make_shared<stream_event_t>(*this);
}

void stream_event_t::read(vnx::TypeInput& _in, const vnx::TypeCode* _type_code, const uint16_t* _code) {
	vnx::read(_in, *this, _type_code, _code);
}

void stream_event_t::write(vnx::TypeOutput& _out, const vnx::TypeCode* _type_code, const uint16_t* _code) const {
	vnx::write(_out, *this, _type_code, _code);
}

void stream_event_t::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, stream);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, event);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, value);
	_visitor.type_end(*_type_code);
}

void stream_event_t::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"stream\": "; vnx::write(_out, stream);
	_out << ", \"event\": "; vnx::write(_out, event);
	_out << ", \"value\": "; vnx::write(_out, value);
	_out << "}";
}

void stream_event_t::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "event") {
			vnx::from_string(_entry.second, event);
		} else if(_entry.first == "stream") {
			vnx::from_string(_entry.second, stream);
		} else if(_entry.first == "value") {
			vnx::from_string(_entry.second, value);
		}
	}
}

vnx::Object stream_event_t::to_object() const {
	vnx::Object _object;
	_object["stream"] = stream;
	_object["event"] = event;
	_object["value"] = value;
	return _object;
}

void stream_event_t::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "event") {
			_entry.second.to(event);
		} else if(_entry.first == "stream") {
			_entry.second.to(stream);
		} else if(_entry.first == "value") {
			_entry.second.to(value);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const stream_event_t& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, stream_event_t& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* stream_event_t::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xd4f7a89285ca745aull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> stream_event_t::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.stream_event_t";
	type_code->type_hash = vnx::Hash64(0xd4f7a89285ca745aull);
	type_code->code_hash = vnx::Hash64(0x471264f930a696f9ull);
	type_code->create_value = []() -> std::shared_ptr<vnx::Value> { return std::make_shared<vnx::Struct<stream_event_t>>(); };
	type_code->fields.resize(3);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "stream";
		field.code = {11, 2, 4};
	}
	{
		vnx::TypeField& field = type_code->fields[1];
		field.name = "event";
		field.value = vnx::to_string(0);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[2];
		field.name = "value";
		field.value = vnx::to_string(0);
		field.code = {7};
	}
	type_code->build();
	return type_code;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::stream_event_t& value, const TypeCode* type_code, const uint16_t* code) {
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
		const vnx::TypeField* const _field = type_code->field_map[1];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.event, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[2];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.value, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.stream, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::stream_event_t& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::stream_event_t>(out);
		vnx::write_class_header<::vnx::web::stream_event_t>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(8);
	vnx::write_value(_buf + 0, value.event);
	vnx::write_value(_buf + 4, value.value);
	vnx::write(out, value.stream, type_code, type_code->fields[0].code.data());
}

void read(std::istream& in, ::vnx::web::stream_event_t& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::stream_event_t& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::stream_event_t& value) {
	value.accept(visitor);
}

} // vnx
