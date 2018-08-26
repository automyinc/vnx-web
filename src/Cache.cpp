
#include <vnx/web/Cache.h>


namespace vnx {
namespace web {

Cache::Cache(const std::string& _vnx_name)
	:	CacheBase(_vnx_name)
{
}

void Cache::init() {
	input_pipe = subscribe(input);
	subscribe(domain);
	subscribe(channel, 0);
}

void Cache::main() {
	cache_salt = Hash64::rand();
	provider_id = Hash128::rand();
	table.resize(num_entries);
	
	set_timer_millis(update_interval_ms, std::bind(&Cache::update, this));
	set_timer_millis(maintain_interval_ms, std::bind(&Cache::maintain, this));
	set_timer_millis(1000, std::bind(&Cache::print_stats, this));
	
	Super::main();
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Provider> provider) {
	if(provider->id != provider_id && !provider->path.empty()) {
		Path path = provider->path;
		if(path.back().empty()) {
			path.pop_back();
		}
		provider_map[path] = provider;
	}
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	request_counter++;
	const int64_t now = vnx::get_time_millis();
	if(request->type == request_type_e::READ) {
		read_request_counter++;
		cache_entry_t& entry = cache_lookup(request->path);
		if(	entry.content
			&& request->path == entry.path
			&& now < entry.time_stamp_ms + entry.time_to_live_ms)
		{
			std::shared_ptr<Response> response = Response::create();
			response->is_for_request(request);
			response->content = entry.content;
			response->time_to_live_ms = entry.time_to_live_ms;
			publish(response, request->get_return_channel());
			
			entry.num_hits++;
			entry.last_request_ms = now;
			hit_counter++;
			return;
		}
	}
	if(request->type == request_type_e::WRITE) {
		cache_entry_t& entry = cache_lookup(request->path);
		if(	entry.content
			&& request->path == entry.path)
		{
			entry = cache_entry_t();	// purge old data
		}
	}
	if(now > request->time_stamp_ms + request->timeout_ms) {
		publish(Response::create(request, ErrorCode::create(ErrorCode::TIMEOUT)), request->get_return_channel());
		return;
	}
	std::shared_ptr<const Provider> provider = find_provider(request->path);
	if(provider) {
		std::shared_ptr<Request> new_request = Request::create();
		new_request->forward_relative(request, channel, provider->path);
		publish(new_request, provider->input);
		push_request(new_request->id, request);
		return;
	}
	publish(Response::create(request, ErrorCode::create(ErrorCode::NOT_FOUND)), request->get_return_channel());
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Response> response) {
	auto iter = pending_requests.find(response->id);
	if(iter != pending_requests.end()) {
		std::shared_ptr<const Request> request = iter->second;
		
		std::shared_ptr<Response> new_response = Response::create();
		new_response->forward(response);
		publish(new_response, response->get_return_channel());
		
		if(	!response->is_dynamic
			&& response->content
			&& response->content->get_num_bytes() <= max_entry_size)
		{
			cache_entry_t& entry = cache_lookup(request->path);
			if(entry.content) {
				if(request->path != entry.path) {
					entry = cache_entry_t();
				}
			}
			entry.path = request->path;
			entry.content = response->content;
			entry.time_stamp_ms = vnx::get_time_millis();
			entry.time_to_live_ms = response->time_to_live_ms;
			entry.last_request_ms = entry.time_stamp_ms;
		}
		
		erase_request(response->id);
	}
}

void Cache::purge() {
	for(cache_entry_t& entry : table) {
		entry = cache_entry_t();
	}
}

cache_entry_t& Cache::cache_lookup(const Path& path) {
	return table[Hash64(path.get_hash(), cache_salt) % num_entries];
}

std::shared_ptr<const Provider> Cache::find_provider(const Path& path) {
	for(size_t N = path.size(); N > 0; --N) {
		auto iter = provider_map.find(path.get_base_path(N));
		if(iter != provider_map.end()) {
			return iter->second;
		}
	}
	return 0;
}

void Cache::push_request(const Hash128& id, std::shared_ptr<const Request> request) {
	pending_requests[id] = request;
	if(input_pipe) {
		const size_t backlog = pending_requests.size();
		if(backlog >= max_pending) {
			if(!input_pipe->get_is_paused()) {
				log(WARN).out << "Blocked input: pending=" << backlog;
			}
			input_pipe->pause();
		}
	}
}

void Cache::erase_request(const Hash128& id) {
	pending_requests.erase(id);
	if(input_pipe) {
		const size_t backlog = pending_requests.size();
		if(backlog <= max_pending / 2) {
			if(input_pipe->get_is_paused()) {
				log(WARN).out << "Resumed input: pending=" << backlog;
			}
			input_pipe->resume();
		}
	}
}

void Cache::update() {
	const int64_t now = vnx::get_time_millis();
	{
		std::vector<Hash128> list;
		for(const auto& entry : pending_requests) {
			if(now > entry.second->time_stamp_ms + entry.second->timeout_ms) {
				list.push_back(entry.first);
			}
		}
		for(const Hash128& id : list) {
			auto request = pending_requests[id];
			publish(Response::create(request, ErrorCode::create(ErrorCode::TIMEOUT)), request->get_return_channel());
			erase_request(id);
		}
		num_timeout += list.size();
	}
	for(const auto& entry : provider_map) {
		auto provider = vnx::clone(entry.second);
		provider->id = provider_id;
		provider->input = channel;
		provider->level = entry.second->level + 1;
		publish(provider, domain);
	}
}

void Cache::maintain() {
	const int64_t now = vnx::get_time_millis();
	for(cache_entry_t& entry : table) {
		if(entry.content) {
			if(now > entry.time_stamp_ms + entry.time_to_live_ms) {
				entry = cache_entry_t();
			}
		}
	}
}

void Cache::print_stats() {
	log(INFO).out << "requests=" << request_counter << "/s, pending=" << pending_requests.size()
			<< ", num_timeout=" << num_timeout << ", hitrate=" << (100 * float(hit_counter) / float(read_request_counter)) << " %";
	request_counter = 0;
	read_request_counter = 0;
	hit_counter = 0;
	num_timeout = 0;
}


} // web
} // vnx
