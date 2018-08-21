
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
	
	Super::main();
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Content> content) {
	cache_entry_t& entry = cache_lookup(content->path);
	if(	entry.content
		&& content->path == entry.content->path
		&& content->time_stamp_ms >= entry.content->time_stamp_ms)
	{
		entry.content = content;
	}
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Provider> provider) {
	if(provider->id != provider_id) {
		provider_map[provider->path] = provider;
	}
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Request> request) {
	request_counter++;
	const int64_t now = vnx::get_time_millis();
	if(request->type == request_type_e::READ) {
		cache_entry_t& entry = cache_lookup(request->path);
		if(	entry.content
			&& request->path == entry.content->path
			&& now < entry.time_stamp_ms + entry.time_to_live_ms)
		{
			std::shared_ptr<Response> response = Response::create();
			response->id = request->id;
			response->content = entry.content;
			response->time_to_live_ms = entry.time_to_live_ms;
			publish(response, request->channel);
			
			// check if to pre-fetch
			if(now - entry.last_request_ms > entry.time_to_live_ms / 2) {
				std::shared_ptr<const Provider> provider = find_provider(request->path);
				if(provider) {
					std::shared_ptr<Request> forward = vnx::clone(request);
					forward->channel = channel;
					publish(forward, provider->channel);
					entry.last_request_ms = now;
				}
			}
			entry.num_hits++;
			hit_counter++;
			return;
		}
	}
	if(request->type == request_type_e::WRITE) {
		cache_entry_t& entry = cache_lookup(request->path);
		if(	entry.content
			&& request->path == entry.content->path)
		{
			entry = cache_entry_t();	// purge old data
		}
	}
	if(now > request->time_stamp_ms + request->timeout_ms) {
		publish(Response::create(request, ErrorCode::create(ErrorCode::TIMEOUT)), request->channel);
		return;
	}
	std::shared_ptr<const Provider> provider = find_provider(request->path);
	if(provider) {
		std::shared_ptr<Request> forward = vnx::clone(request);
		forward->channel = channel;
//		publish(forward, provider->channel);
		push_request(request);
		return;
	}
	publish(Response::create(request, ErrorCode::create(ErrorCode::NOT_FOUND)), request->channel);
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Response> response) {
	auto iter = pending_requests.find(response->id);
	if(iter != pending_requests.end()) {
		publish(response, iter->second->channel);
		erase_request(response->id);
	}
	if(	!response->is_dynamic
		&& response->content
		&& response->content->get_num_bytes() <= max_entry_size)
	{
		cache_entry_t& entry = cache_lookup(response->content->path);
		if(entry.content) {
			if(response->content->path != entry.content->path) {
				entry = cache_entry_t();
			}
		}
		entry.content = response->content;
		entry.time_stamp_ms = vnx::get_time_millis();
		entry.time_to_live_ms = response->time_to_live_ms;
		entry.last_request_ms = entry.time_stamp_ms;
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

void Cache::push_request(std::shared_ptr<const Request> request) {
	pending_requests[request->id] = request;
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
	size_t num_timeout = 0;
	{
		std::vector<std::shared_ptr<const Request>> list;
		for(const auto& entry : pending_requests) {
			if(now > entry.second->time_stamp_ms + entry.second->timeout_ms) {
				list.push_back(entry.second);
			}
		}
		for(const auto& request : list) {
			publish(Response::create(request, ErrorCode::create(ErrorCode::TIMEOUT)), request->channel);
			erase_request(request->id);
		}
		num_timeout += list.size();
	}
	for(const auto& entry : provider_map) {
		auto provider = vnx::clone(entry.second);
		provider->id = provider_id;
		provider->channel = channel;
		provider->level = entry.second->level + 1;
		publish(provider, domain);
	}
	log(INFO).out << "requests=" << ((1000 * request_counter) / update_interval_ms) << "/s, hitrate="
			<< (100 * float(hit_counter) / float(request_counter)) << " %, pending="
			<< pending_requests.size() << ", num_timeout=" << num_timeout;
	request_counter = 0;
	hit_counter = 0;
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


} // web
} // vnx
