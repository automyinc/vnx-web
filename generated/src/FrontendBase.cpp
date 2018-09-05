
// AUTO GENERATED by vnxcppcodegen

#include <vnx/web/package.hxx>
#include <vnx/web/FrontendBase.hxx>
#include <vnx/Input.h>
#include <vnx/Output.h>
#include <vnx/Visitor.h>
#include <vnx/Object.h>
#include <vnx/Struct.h>
#include <vnx/Config.h>


namespace vnx {
namespace web {


const vnx::Hash64 FrontendBase::VNX_TYPE_HASH(0xc9fab0af74907466ull);
const vnx::Hash64 FrontendBase::VNX_CODE_HASH(0x2fbd209da3fe5af7ull);

FrontendBase::FrontendBase(const std::string& _vnx_name)
	:	Module::Module(_vnx_name)
{
	vnx::read_config(vnx_name + ".address", address);
	vnx::read_config(vnx_name + ".channel", channel);
	vnx::read_config(vnx_name + ".connection_timeout_ms", connection_timeout_ms);
	vnx::read_config(vnx_name + ".input", input);
	vnx::read_config(vnx_name + ".listen_queue_size", listen_queue_size);
	vnx::read_config(vnx_name + ".max_write_backlog", max_write_backlog);
	vnx::read_config(vnx_name + ".output", output);
	vnx::read_config(vnx_name + ".read_block_size", read_block_size);
	vnx::read_config(vnx_name + ".write_block_size", write_block_size);
}

vnx::Hash64 FrontendBase::get_type_hash() const {
	return VNX_TYPE_HASH;
}

const char* FrontendBase::get_type_name() const {
	return "vnx.web.Frontend";
}

void FrontendBase::accept(vnx::Visitor& _visitor) const {
	const vnx::TypeCode* _type_code = get_type_code();
	_visitor.type_begin(*_type_code);
	_visitor.type_field(_type_code->fields[0], 0); vnx::accept(_visitor, address);
	_visitor.type_field(_type_code->fields[1], 1); vnx::accept(_visitor, input);
	_visitor.type_field(_type_code->fields[2], 2); vnx::accept(_visitor, output);
	_visitor.type_field(_type_code->fields[3], 3); vnx::accept(_visitor, channel);
	_visitor.type_field(_type_code->fields[4], 4); vnx::accept(_visitor, listen_queue_size);
	_visitor.type_field(_type_code->fields[5], 5); vnx::accept(_visitor, connection_timeout_ms);
	_visitor.type_field(_type_code->fields[6], 6); vnx::accept(_visitor, max_write_backlog);
	_visitor.type_field(_type_code->fields[7], 7); vnx::accept(_visitor, read_block_size);
	_visitor.type_field(_type_code->fields[8], 8); vnx::accept(_visitor, write_block_size);
	_visitor.type_end(*_type_code);
}

void FrontendBase::write(std::ostream& _out) const {
	_out << "{";
	_out << "\"address\": "; vnx::write(_out, address);
	_out << ", \"input\": "; vnx::write(_out, input);
	_out << ", \"output\": "; vnx::write(_out, output);
	_out << ", \"channel\": "; vnx::write(_out, channel);
	_out << ", \"listen_queue_size\": "; vnx::write(_out, listen_queue_size);
	_out << ", \"connection_timeout_ms\": "; vnx::write(_out, connection_timeout_ms);
	_out << ", \"max_write_backlog\": "; vnx::write(_out, max_write_backlog);
	_out << ", \"read_block_size\": "; vnx::write(_out, read_block_size);
	_out << ", \"write_block_size\": "; vnx::write(_out, write_block_size);
	_out << "}";
}

void FrontendBase::read(std::istream& _in) {
	std::map<std::string, std::string> _object;
	vnx::read_object(_in, _object);
	for(const auto& _entry : _object) {
		if(_entry.first == "address") {
			vnx::from_string(_entry.second, address);
		} else if(_entry.first == "channel") {
			vnx::from_string(_entry.second, channel);
		} else if(_entry.first == "connection_timeout_ms") {
			vnx::from_string(_entry.second, connection_timeout_ms);
		} else if(_entry.first == "input") {
			vnx::from_string(_entry.second, input);
		} else if(_entry.first == "listen_queue_size") {
			vnx::from_string(_entry.second, listen_queue_size);
		} else if(_entry.first == "max_write_backlog") {
			vnx::from_string(_entry.second, max_write_backlog);
		} else if(_entry.first == "output") {
			vnx::from_string(_entry.second, output);
		} else if(_entry.first == "read_block_size") {
			vnx::from_string(_entry.second, read_block_size);
		} else if(_entry.first == "write_block_size") {
			vnx::from_string(_entry.second, write_block_size);
		}
	}
}

vnx::Object FrontendBase::to_object() const {
	vnx::Object _object;
	_object["address"] = address;
	_object["input"] = input;
	_object["output"] = output;
	_object["channel"] = channel;
	_object["listen_queue_size"] = listen_queue_size;
	_object["connection_timeout_ms"] = connection_timeout_ms;
	_object["max_write_backlog"] = max_write_backlog;
	_object["read_block_size"] = read_block_size;
	_object["write_block_size"] = write_block_size;
	return _object;
}

void FrontendBase::from_object(const vnx::Object& _object) {
	for(const auto& _entry : _object.field) {
		if(_entry.first == "address") {
			_entry.second.to(address);
		} else if(_entry.first == "channel") {
			_entry.second.to(channel);
		} else if(_entry.first == "connection_timeout_ms") {
			_entry.second.to(connection_timeout_ms);
		} else if(_entry.first == "input") {
			_entry.second.to(input);
		} else if(_entry.first == "listen_queue_size") {
			_entry.second.to(listen_queue_size);
		} else if(_entry.first == "max_write_backlog") {
			_entry.second.to(max_write_backlog);
		} else if(_entry.first == "output") {
			_entry.second.to(output);
		} else if(_entry.first == "read_block_size") {
			_entry.second.to(read_block_size);
		} else if(_entry.first == "write_block_size") {
			_entry.second.to(write_block_size);
		}
	}
}

std::ostream& operator<<(std::ostream& _out, const FrontendBase& _value) {
	_value.write(_out);
	return _out;
}

std::istream& operator>>(std::istream& _in, FrontendBase& _value) {
	_value.read(_in);
	return _in;
}

const vnx::TypeCode* FrontendBase::get_type_code() {
	const vnx::TypeCode* type_code = vnx::get_type_code(vnx::Hash64(0xc9fab0af74907466ull));
	if(!type_code) {
		type_code = vnx::register_type_code(create_type_code());
	}
	return type_code;
}

std::shared_ptr<vnx::TypeCode> FrontendBase::create_type_code() {
	std::shared_ptr<vnx::TypeCode> type_code = std::make_shared<vnx::TypeCode>(true);
	type_code->name = "vnx.web.Frontend";
	type_code->type_hash = vnx::Hash64(0xc9fab0af74907466ull);
	type_code->code_hash = vnx::Hash64(0x2fbd209da3fe5af7ull);
	type_code->methods.resize(0);
	type_code->fields.resize(9);
	{
		vnx::TypeField& field = type_code->fields[0];
		field.is_extended = true;
		field.name = "address";
		field.value = vnx::to_string("127.0.0.1:8080");
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
		field.name = "output";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[3];
		field.is_extended = true;
		field.name = "channel";
		field.code = {12, 5};
	}
	{
		vnx::TypeField& field = type_code->fields[4];
		field.name = "listen_queue_size";
		field.value = vnx::to_string(256);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[5];
		field.name = "connection_timeout_ms";
		field.value = vnx::to_string(10000);
		field.code = {7};
	}
	{
		vnx::TypeField& field = type_code->fields[6];
		field.name = "max_write_backlog";
		field.value = vnx::to_string(268435456);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[7];
		field.name = "read_block_size";
		field.value = vnx::to_string(16384);
		field.code = {8};
	}
	{
		vnx::TypeField& field = type_code->fields[8];
		field.name = "write_block_size";
		field.value = vnx::to_string(65536);
		field.code = {8};
	}
	type_code->build();
	return type_code;
}

void FrontendBase::handle_switch(std::shared_ptr<const ::vnx::Sample> _sample) {
	const uint64_t _type_hash = _sample->value->get_type_hash();
}

bool FrontendBase::call_switch(vnx::TypeInput& _in, vnx::TypeOutput& _out, const vnx::TypeCode* _call_type, const vnx::TypeCode* _return_type) {
	return false;
}


} // namespace vnx
} // namespace web


namespace vnx {

void read(TypeInput& in, ::vnx::web::FrontendBase& value, const TypeCode* type_code, const uint16_t* code) {
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
			vnx::read_value(_buf + _field->offset, value.listen_queue_size, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[5];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.connection_timeout_ms, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[6];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.max_write_backlog, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[7];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.read_block_size, _field->code.data());
		}
	}
	{
		const vnx::TypeField* const _field = type_code->field_map[8];
		if(_field) {
			vnx::read_value(_buf + _field->offset, value.write_block_size, _field->code.data());
		}
	}
	for(const vnx::TypeField* _field : type_code->ext_fields) {
		switch(_field->native_index) {
			case 0: vnx::read(in, value.address, type_code, _field->code.data()); break;
			case 1: vnx::read(in, value.input, type_code, _field->code.data()); break;
			case 2: vnx::read(in, value.output, type_code, _field->code.data()); break;
			case 3: vnx::read(in, value.channel, type_code, _field->code.data()); break;
			default: vnx::skip(in, type_code, _field->code.data());
		}
	}
}

void write(TypeOutput& out, const ::vnx::web::FrontendBase& value, const TypeCode* type_code, const uint16_t* code) {
	if(!type_code || (code && code[0] == CODE_ANY)) {
		type_code = vnx::write_type_code<::vnx::web::FrontendBase>(out);
		vnx::write_class_header<::vnx::web::FrontendBase>(out);
	}
	if(code && code[0] == CODE_STRUCT) {
		type_code = type_code->depends[code[1]];
	}
	char* const _buf = out.write(32);
	vnx::write_value(_buf + 0, value.listen_queue_size);
	vnx::write_value(_buf + 4, value.connection_timeout_ms);
	vnx::write_value(_buf + 8, value.max_write_backlog);
	vnx::write_value(_buf + 16, value.read_block_size);
	vnx::write_value(_buf + 24, value.write_block_size);
	vnx::write(out, value.address, type_code, type_code->fields[0].code.data());
	vnx::write(out, value.input, type_code, type_code->fields[1].code.data());
	vnx::write(out, value.output, type_code, type_code->fields[2].code.data());
	vnx::write(out, value.channel, type_code, type_code->fields[3].code.data());
}

void read(std::istream& in, ::vnx::web::FrontendBase& value) {
	value.read(in);
}

void write(std::ostream& out, const ::vnx::web::FrontendBase& value) {
	value.write(out);
}

void accept(Visitor& visitor, const ::vnx::web::FrontendBase& value) {
	value.accept(visitor);
}

} // vnx
