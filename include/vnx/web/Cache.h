
#ifndef INCLUDE_VNX_WEB_CACHE_H
#define INCLUDE_VNX_WEB_CACHE_H

#include <vnx/web/CacheBase.hxx>
#include <vnx/web/CacheEntry.hxx>
#include <vnx/web/ErrorCode.hxx>

#include <unordered_map>


namespace vnx {
namespace web {

class Cache : public CacheBase {
public:
	Cache(const std::string& _vnx_name);
	
protected:
	void main() override {
		
		table.resize(num_entries);
		
		subscribe(channel);
		subscribe(domain);
		
		set_timer_millis(1000, std::bind(&Cache::update, this));
		
		Super::main();
	}
	
	void handle(std::shared_ptr<const ::vnx::web::Content> content) override {
		CacheEntry& entry = cache_lookup(content->path);
		if(	entry.content
			&& content->path == entry.content->path
			&& content->time_stamp_ms >= entry.content->time_stamp_ms)
		{
			entry.content = content;
		}
	}
	
	void handle(std::shared_ptr<const ::vnx::web::Provider> value) override {
		provider_map[value->path.to_string()] = value;
	}
	
	void handle(std::shared_ptr<const ::vnx::web::Request> request) override {
		{
			CacheEntry& entry = cache_lookup(request->path);
			if(	entry.content
				&& request->path == entry.content->path
				&& request->time_stamp_ms < entry.content->time_stamp_ms + entry.time_to_live_ms)
			{
				std::shared_ptr<Response> response = Response::create();
				response->id = request->id;
				response->content = entry.content;
				response->time_to_live_ms = entry.time_to_live_ms;
				publish(response, request->channel);
				entry.num_hits++;
				return;
			}
		}
		{
			const Path& path = request->path;
			for(size_t N = path.size(); N > 0; --N) {
				auto iter = provider_map.find(path.to_string(N));
				if(iter != provider_map.end()) {
					forward_request(request, iter->second);
					return;
				}
			}
		}
		std::shared_ptr<Response> response = Response::create();
		response->id = request->id;
		response->content = ErrorCode::create(ErrorCode::NOT_FOUND);
		publish(response, request->channel);
	}
	
	void handle(std::shared_ptr<const ::vnx::web::Response> response) override {
		auto iter = pending_requests.find(response->id);
		if(iter != pending_requests.end()) {
			process_response(iter->second, response);
			pending_requests.erase(iter);
		}
	}
	
	void purge() override {
		for(CacheEntry& entry : table) {
			entry = CacheEntry();
		}
	}
	
private:
	void forward_request(std::shared_ptr<const Request> request, std::shared_ptr<const Provider> provider) {
		std::shared_ptr<Request> forward = vnx::clone(request);
		forward->channel = channel;
		publish(forward, provider->channel);
		pending_requests[request->id] = request;
	}
	
	void process_response(std::shared_ptr<const Request> request, std::shared_ptr<const Response> response) {
		publish(response, request->channel);
		if(!response->is_dynamic) {
			CacheEntry& entry = cache_lookup(response->content->path);
			if(entry.content) {
				if(response->content->path != entry.content->path) {
					entry = CacheEntry();
				}
			}
			entry.content = response->content;
			entry.time_to_live_ms = response->time_to_live_ms;
		}
	}
	
	CacheEntry& cache_lookup(const Path& path) {
		return table[path.get_hash() % num_entries];
	}
	
	void update() {
		const int64_t now = vnx::get_time_millis();
		{
			std::vector<Hash128> list;
			for(const auto& entry : pending_requests) {
				if(now > entry.second->time_stamp_ms + entry.second->timeout_ms) {
					list.push_back(entry.first);
				}
			}
			for(const Hash128& key : list) {
				std::shared_ptr<const Request> request = pending_requests[key];
				std::shared_ptr<Response> response = Response::create();
				response->id = request->id;
				response->content = ErrorCode::create(ErrorCode::TIMEOUT);
				publish(response, request->channel);
				pending_requests.erase(key);
			}
		}
		for(CacheEntry& entry : table) {
			if(entry.content) {
				if(now > entry.content->time_stamp_ms + entry.time_to_live_ms) {
					entry = CacheEntry();
				}
			}
		}
	}
	
private:
	std::vector<CacheEntry> table;
	std::unordered_map<std::string, std::shared_ptr<const Provider>> provider_map;
	std::unordered_map<Hash128, std::shared_ptr<const Request>> pending_requests;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_CACHE_H
