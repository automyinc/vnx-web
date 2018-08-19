
#include <vnx/web/Cache.h>


namespace vnx {
namespace web {

Cache::Cache(const std::string& _vnx_name)
	:	CacheBase(_vnx_name)
{
}

void Cache::main() {
	cache_salt = Hash64::rand();
	provider_id = Hash128::rand();
	table.resize(num_entries);
	
	subscribe(channel);
	subscribe(domain);
	
	set_timer_millis(update_interval_ms, std::bind(&Cache::update, this));
	set_timer_millis(maintain_interval_ms, std::bind(&Cache::maintain, this));
	
	Super::main();
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Content> content) {
	CacheEntry& entry = cache_lookup(content->path);
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
	const int64_t now = vnx::get_time_millis();
	if(!request->parameter) {
		CacheEntry& entry = cache_lookup(request->path);
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
			return;
		}
	}
	if(now > request->time_stamp_ms + request->timeout_ms) {
		publish(Response::create(request, ErrorCode::create(ErrorCode::TIMEOUT)), request->channel);
		return;
	}
	{
		auto iter = resource_backlog.find(request->path);
		if(iter != resource_backlog.end()) {
			if(iter->second >= max_backlog) {
				publish(Response::create(request, ErrorCode::create(ErrorCode::OVERLOAD)), request->channel);
				return;
			}
		}
	}
	std::shared_ptr<const Provider> provider = find_provider(request->path);
	if(provider) {
		std::shared_ptr<Request> forward = vnx::clone(request);
		forward->channel = channel;
		publish(forward, provider->channel);
		add_request(request);
		return;
	}
	publish(Response::create(request, ErrorCode::create(ErrorCode::NOT_FOUND)), request->channel);
}

void Cache::handle(std::shared_ptr<const ::vnx::web::Response> response) {
	auto iter = pending_requests.find(response->id);
	if(iter != pending_requests.end()) {
		publish(response, iter->second->channel);
		rem_request(iter->second);
	}
	if(	!response->is_dynamic
		&& response->content->get_num_bytes() <= max_entry_size)
	{
		CacheEntry& entry = cache_lookup(response->content->path);
		if(entry.content) {
			if(response->content->path != entry.content->path) {
				entry = CacheEntry();
			}
		}
		entry.content = response->content;
		entry.time_stamp_ms = vnx::get_time_millis();
		entry.time_to_live_ms = response->time_to_live_ms;
		entry.last_request_ms = entry.time_stamp_ms;
	}
}

void Cache::purge() {
	for(CacheEntry& entry : table) {
		entry = CacheEntry();
	}
}

CacheEntry& Cache::cache_lookup(const Path& path) {
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

void Cache::add_request(std::shared_ptr<const Request> request) {
	pending_requests[request->id] = request;
	resource_backlog[request->path]++;
}

void Cache::rem_request(std::shared_ptr<const Request> request) {
	pending_requests.erase(request->id);
	if(--resource_backlog[request->path] <= 0) {
		resource_backlog.erase(request->path);
	}
}

void Cache::update() {
	const int64_t now = vnx::get_time_millis();
	{
		std::vector<std::shared_ptr<const Request>> list;
		for(const auto& entry : pending_requests) {
			if(now > entry.second->time_stamp_ms + entry.second->timeout_ms) {
				list.push_back(entry.second);
			}
		}
		for(const auto& request : list) {
			publish(Response::create(request, ErrorCode::create(ErrorCode::TIMEOUT)), request->channel);
			rem_request(request);
		}
	}
	for(const auto& entry : provider_map) {
		auto provider = vnx::clone(entry.second);
		provider->id = provider_id;
		provider->channel = channel;
		provider->level = entry.second->level + 1;
		publish(provider, domain);
	}
}

void Cache::maintain() {
	const int64_t now = vnx::get_time_millis();
	for(CacheEntry& entry : table) {
		if(entry.content) {
			if(now > entry.time_stamp_ms + entry.time_to_live_ms) {
				entry = CacheEntry();
			}
		}
	}
}


} // web
} // vnx
