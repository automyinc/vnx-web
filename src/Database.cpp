
#include <vnx/web/Database.h>
#include <vnx/Request.hxx>
#include <vnx/Generic.hxx>


namespace vnx {
namespace web {

Database::Database(const std::string& _vnx_name)
		: DatabaseBase(_vnx_name) {
}

void Database::init() {
	open_pipe(vnx_name, this, 1000);
}

void Database::main() {
	{
		auto value = vnx::read_from_file<vnx::Generic>(location + "index.dat");
		if(value) {
			auto list = value->data.to<std::vector<std::string>>();
			if(!list.empty()) {
				for(const auto& file_name : list) {
					block_t block;
					block.file = std::make_shared<vnx::File>(file_name);
					read_block(block, block_table.size());
					block_table.push_back(block);
				}
				log(INFO).out << "Loaded " << block_table.size() << " blocks, " << index.size() << " objects total.";
			} else {
				throw std::runtime_error("Invalid index file!");
			}
		}
	}
	{
		auto value = vnx::read_from_file<vnx::Generic>(location + "table.dat");
		if(value) {
			auto list = value->data.to<std::vector<std::string>>();
			if(!list.empty()) {
				for(const auto& table_name : list) {
					table_t& table = get_table(table_name);
					table.hash = Hash128(table_name);
					table.file = std::make_shared<vnx::File>(location + "table_" + table_name + ".dat");
					read_table(table);
					log(INFO).out << "Loaded table '" << table_name << "' with " << table.rows.size() << " objects total.";
				}
			} else {
				throw std::runtime_error("Invalid table file!");
			}
		}
	}
	
	stage_file.open(location + "stage.dat", "ab+");
	stage_file.seek_begin();
	size_t last_pos = 0;
	try {
		while(true) {
			std::shared_ptr<Value> value = vnx::read(stage_file.in);
			if(!value) {
				break;
			}
			auto request = std::dynamic_pointer_cast<const vnx::Request>(value);
			if(request) {
				Super::handle(request);
			}
			last_pos = stage_file.get_input_pos();
		}
	} catch(const std::exception& ex) {
		if(last_pos > 0) {
			log(WARN).out << ex.what();
		}
	}
	stage_file.close();
	log(INFO).out << "Loaded " << stage.size() << " objects from stage file.";
	
	stage_file.open("rb+");
	stage_file.seek_to(last_pos);
	is_replay = false;
	
	set_timer_millis(1000, std::bind(&Database::maintain, this));
	
	Super::main();
	
	vnx::write(stage_file.out, std::shared_ptr<Value>());
	stage_file.close();
}

std::shared_ptr<const vnx::Return> Database::handle(std::shared_ptr<const vnx::Request> request) {
	if(is_replay) {
		Super::handle(request);
		return 0;
	} else {
		current = request;
		return Super::handle(request);
	}
}

static Object aggregate(const query::Select& query, const std::vector<Object>& result) {
	std::map<std::string, std::shared_ptr<query::Aggregate>> funcs;
	for(const auto& entry : query.aggregates) {
		funcs[entry.first] = vnx::clone(entry.second);
	}
	for(const auto& object : result) {
		for(auto& entry : funcs) {
			entry.second->update(object);
		}
	}
	Object row;
	for(auto& entry : funcs) {
		row[entry.first] = entry.second->get_result();
	}
	return row;
}

std::vector<Object> Database::select(const query::Select& query) const {
	const table_t& table = find_table(query.from);
	std::vector<Object> result;
	for(const auto& key : table.rows) {
		Object object;
		if(get_object(key, object)) {
			object["id"] = flip_hash(table, key);
		}
		if(!query.where || query.where->execute(object).to<bool>()) {
			if(query.fields.empty()) {
				result.push_back(std::move(object));
			} else {
				result.emplace_back();
				Object& row = result.back();
				for(const auto& entry : query.fields) {
					if(entry.second) {
						row[entry.first] = entry.second->execute(object);
					}
				}
			}
		}
	}
	if(query.group_by) {
		if(!query.fields.empty()) {
			throw std::logic_error("cannot have fields in group_by mode");
		}
		std::unordered_map<Variant, std::vector<Object>> groups;
		for(auto& object : result) {
			groups[object[query.group_by->field]].push_back(std::move(object));
		}
		result.clear();
		for(const auto& entry : groups) {
			Object object = aggregate(query, entry.second);
			if(!query.having || query.having->execute(object).to<bool>()) {
				object[query.group_by->field] = entry.first;
				result.push_back(std::move(object));
			}
		}
	} else {
		if(!query.aggregates.empty()) {
			if(!query.fields.empty()) {
				throw std::logic_error("cannot have fields in aggregate mode");
			}
			Object object = aggregate(query, result);
			result.clear();
			result.push_back(std::move(object));
		}
	}
	if(query.order_by) {
		std::multimap<Variant, Object> index;
		for(auto& object : result) {
			index.insert(std::make_pair(object[query.order_by->field], std::move(object)));
		}
		result.clear();
		switch(query.order_by->mode) {
			case query::OrderBy::ASC:
				for(auto it = index.begin(); it != index.end(); ++it) {
					result.push_back(std::move(it->second));
				}
				break;
			case query::OrderBy::DESC:
				for(auto it = index.rbegin(); it != index.rend(); ++it) {
					result.push_back(std::move(it->second));
				}
				break;
			default:
				throw std::logic_error("invalid order_by mode");
		}
	}
	if(query.limit) {
		if(query.limit->offset > 0) {
			result.erase(result.begin(), result.begin() + query.limit->offset);
		}
		if(query.limit->num_rows >= 0) {
			if(result.size() > query.limit->num_rows) {
				result.resize(size_t(query.limit->num_rows));
			}
		}
	}
	return result;
}

void Database::update(const query::Update& query) {
	const table_t& table = find_table(query.table);
	for(const auto& key : table.rows) {
		Object object;
		if(get_object(key, object)) {
			object["id"] = flip_hash(table, key);
		}
		if(!query.where || query.where->execute(object).to<bool>()) {
			update_one(key, query.set);
		}
	}
	commit();
}

void Database::delete_from(const query::Delete& query) {
	if(query.where) {
		const table_t& table = find_table(query.from);
		std::vector<Hash128> list;
		for(const auto& key : table.rows) {
			const Hash128 id = flip_hash(table, key);
			Object object;
			if(get_object(key, object)) {
				object["id"] = id;
			}
			if(query.where->execute(object).to<bool>()) {
				list.push_back(id);
			}
		}
		{
			table_t& table = get_table(query.from);
			for(const auto& id : list) {
				delete_one(table, id);
			}
		}
		commit();
	} else {
		truncate(query.from);
	}
}

Object Database::select_one(const std::string& table_name, const Hash128& id) const {
	const table_t& table = find_table(table_name);
	const Hash128 key = flip_hash(table, id);
	Object object;
	if(get_object(key, object)) {
		object["id"] = id;
	}
	return object;
}

std::vector<Object> Database::select_many(const std::string& table_name, const std::vector<Hash128>& ids) const {
	std::vector<Object> result;
	for(const auto& id : ids) {
		result.push_back(select_one(table_name, id));
	}
	return result;
}

void Database::insert_one(table_t& table, const vnx::Hash128& id, const vnx::Object& object) {
	const Hash128 key = flip_hash(table, id);
	insert_key(table, key);
	stage[key] = object;
}

void Database::insert_one(const std::string& table_name, const Hash128& id, const Object& object) {
	table_t& table = get_table(table_name);
	insert_one(table, id, object);
	commit();
}

void Database::insert_many(const std::string& table_name, const std::map<Hash128, Object>& objects) {
	table_t& table = get_table(table_name);
	for(const auto& entry : objects) {
		insert_one(table, entry.first, entry.second);
	}
	commit();
}

void Database::update_one(const Hash128& key, const Object& object) {
	Object *p_object;
	auto iter = stage.find(key);
	if(iter != stage.end()) {
		p_object = &iter->second;
	} else {
		p_object = &stage[key];
		if(!read_object(key, *p_object)) {
			throw std::logic_error("update_one(): no such object");
		}
	}
	for(const auto& field : object.field) {
		(*p_object)[field.first] = field.second;
	}
}

void Database::update_one(table_t& table, const Hash128& id, const Object& object) {
	const Hash128 key = flip_hash(table, id);
	if(insert_key(table, key)) {
		stage[key] = object;
	} else {
		update_one(key, object);
	}
}

void Database::update_one(const std::string& table_name, const Hash128& id, const Object& object) {
	table_t& table = get_table(table_name);
	update_one(table, id, object);
	commit();
}

void Database::update_many(const std::string& table_name, const std::map<Hash128, Object>& objects) {
	table_t& table = get_table(table_name);
	for(const auto& entry : objects) {
		update_one(table, entry.first, entry.second);
	}
	commit();
}

void Database::delete_one(table_t& table, const Hash128& id) {
	const Hash128 key = flip_hash(table, id);
	if(table.rows.count(key)) {
		table.rows.erase(key);
		stage[key].clear();
	}
}

void Database::delete_one(const std::string& table_name, const Hash128& id) {
	table_t& table = get_table(table_name);
	delete_one(table, id);
	commit();
}

void Database::delete_many(const std::string& table_name, const std::vector<Hash128>& ids) {
	table_t& table = get_table(table_name);
	for(const auto& id : ids) {
		delete_one(table, id);
	}
	commit();
}

void Database::truncate(const std::string& table_name) {
	table_t& table = get_table(table_name);
	for(const auto& key : table.rows) {
		stage[key].clear();
	}
	table.rows.clear();
	commit();
}

std::map<std::string, Object> Database::get_table_info() const {
	std::map<std::string, Object> result;
	for(const auto& entry : table_map) {
		Object row;
		row["name"] = entry.first;
		row["num_rows"] = entry.second.rows.size();
		result[entry.first] = row;
	}
	return result;
}

Database::table_t& Database::get_table(const std::string& name) {
	table_t& table = table_map[name];
	if(!table.file) {
		table.hash = Hash128(name);
		table.file = std::make_shared<vnx::File>(location + "table_" + name + ".dat");
	}
	return table;
}

const Database::table_t& Database::find_table(const std::string& name) const {
	auto iter = table_map.find(name);
	if(iter == table_map.end()) {
		throw std::logic_error("Table '" + name + "' not found!");
	}
	return iter->second;
}

Hash128 Database::flip_hash(const table_t& table, const Hash128& hash) const {
	return Hash128(Hash64(hash.A() xor table.hash.A()), Hash64(hash.B() xor table.hash.B()));
}

bool Database::insert_key(table_t& table, const Hash128& key) {
	return table.rows.insert(key).second;
}

void Database::read_block(block_t& block, const size_t block_index) {
	vnx::File& file = *block.file;
	file.open("rb");
	if(file.read_header() != CODE_DYNAMIC) {
		throw std::runtime_error("read_block(): invalid code");
	}
	std::map<Hash128, int64_t> list;
	vnx::read_dynamic(file.in, list);
	for(const auto& entry : list) {
		if(entry.second >= 0) {
			index[entry.first] = std::make_pair(block_index, size_t(entry.second));
		} else {
			index.erase(entry.first);
		}
	}
}

void Database::read_table(table_t& table) {
	vnx::File& file = *table.file;
	file.open("rb");
	if(file.read_header() != CODE_DYNAMIC) {
		throw std::runtime_error("read_table(): invalid code");
	}
	std::vector<Hash128> list;
	vnx::read_dynamic(file.in, list);
	for(const auto& key : list) {
		table.rows.insert(key);
	}
	file.close();
}

void Database::read_object(vnx::File& file, size_t offset, Object& object) const {
	file.seek_to(offset);
	file.in.fetch(1024);
	uint16_t code;
	vnx::read(file.in, code);
	if(code == CODE_OBJECT || code == CODE_ALT_OBJECT) {
		object.read(file.in, 0, &code);
	} else {
		throw std::runtime_error("read_object(): invalid code");
	}
}

bool Database::read_object(const Hash128& key, Object& object) const {
	auto iter = index.find(key);
	if(iter != index.end()) {
		const block_t& block = block_table[iter->second.first];
		if(block.file) {
			read_object(*block.file, iter->second.second, object);
			return true;
		}
	}
	return false;
}

bool Database::get_object(const Hash128& key, Object& object) const {
	auto iter = stage.find(key);
	if(iter != stage.end()) {
		object = iter->second;
		return true;
	}
	return read_object(key, object);
}

void Database::commit() {
	if(current) {
		vnx::write(stage_file.out, current);
		stage_file.flush();
		current = 0;
	}
}

void Database::maintain() {
	if(stage_file.get_output_pos() >= block_size) {
		write_new_block();
	}
}

void Database::write_new_block() {
	
	const size_t block_index = block_table.size();
	block_t block;
	block.file = std::make_shared<vnx::File>();
	vnx::File& file = *block.file;
	file.open(location + "block_" + std::to_string(block_index) + ".dat", "wb");
	file.write_header();
	
	std::map<Hash128, int64_t> header;
	for(const auto& entry : stage) {
		header[entry.first] = -1;
	}
	vnx::write(file.out, uint16_t(CODE_DYNAMIC));
	const size_t header_begin_pos = file.get_output_pos();
	vnx::write_dynamic(file.out, header);
	
	for(const auto& entry : stage) {
		if(!entry.second.empty()) {
			header[entry.first] = int64_t(file.get_output_pos());
			vnx::write(file.out, entry.second);
		}
	}
	file.seek_to(header_begin_pos);
	vnx::write_dynamic(file.out, header);
	file.close();
	
	for(auto& entry : table_map) {
		table_t& table = entry.second;
		table.file->open("wb");
		table.file->write_header();
		vnx::write(table.file->out, uint16_t(CODE_DYNAMIC));
		vnx::write_dynamic(table.file->out, table.rows);
		table.file->close();
	}
	
	file.open("rb");
	read_block(block, block_index);
	block_table.push_back(block);
	stage.clear();
	
	{
		std::vector<std::string> list;
		for(const auto& block_ : block_table) {
			if(block_.file) {
				list.push_back(block_.file->get_path());
			}
		}
		vnx::write_to_file(location + "index.dat", vnx::Generic::create(list));
	}
	{
		std::vector<std::string> list;
		for(const auto& entry : table_map) {
			list.push_back(entry.first);
		}
		vnx::write_to_file(location + "table.dat", vnx::Generic::create(list));
	}
	
	stage_file.close();
	stage_file.open(location + "stage.dat", "wb");
	stage_file.write_header();
	stage_file.flush();
}
	
	
} // web
} // vnx
