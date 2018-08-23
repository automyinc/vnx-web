
#ifndef INCLUDE_VNX_WEB_HISTORYCACHE_H
#define INCLUDE_VNX_WEB_HISTORYCACHE_H

#include <vnx/web/Content.hxx>

#include <queue>
#include <unordered_map>


namespace vnx {
namespace web {

class HistoryCache {
public:
	HistoryCache(size_t max_size_);
	
	std::shared_ptr<const Content> query(const Path& path) const;
	
	void insert(std::shared_ptr<const Content> content);
	
	void remove(const Path& path);
	
	void purge();
	
private:
	size_t max_size = 0;
	std::unordered_map<Path, std::shared_ptr<const Content>> content_map;
	std::queue<Path> history;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_HISTORYCACHE_H
