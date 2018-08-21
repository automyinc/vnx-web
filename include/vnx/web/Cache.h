
#ifndef INCLUDE_VNX_WEB_CACHE_H
#define INCLUDE_VNX_WEB_CACHE_H

#include <vnx/web/CacheBase.hxx>
#include <vnx/web/Provider.hxx>
#include <vnx/web/cache_entry_t.hxx>
#include <vnx/web/ErrorCode.hxx>

#include <unordered_map>


namespace vnx {
namespace web {

class Cache : public CacheBase {
public:
	Cache(const std::string& _vnx_name);
	
protected:
	void init() override;
	
	void main() override;
	
	void handle(std::shared_ptr<const ::vnx::web::Content> content) override;
	
	void handle(std::shared_ptr<const ::vnx::web::Provider> value) override;
	
	void handle(std::shared_ptr<const ::vnx::web::Request> request) override;
	
	void handle(std::shared_ptr<const ::vnx::web::Response> response) override;
	
	void purge() override;
	
private:
	cache_entry_t& cache_lookup(const Path& path);
	
	std::shared_ptr<const Provider> find_provider(const Path& path);
	
	void update();
	
	void maintain();
	
private:
	vnx::Hash64 cache_salt;
	vnx::Hash128 provider_id;
	std::shared_ptr<Pipe> input_pipe;
	std::vector<cache_entry_t> table;
	std::unordered_map<Path, std::shared_ptr<const Provider>> provider_map;
	std::unordered_map<Hash128, std::shared_ptr<const Request>> pending_requests;
	
	size_t request_counter = 0;
	size_t hit_counter = 0;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_CACHE_H
