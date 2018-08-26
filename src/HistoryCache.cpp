
#include <vnx/web/HistoryCache.h>


namespace vnx {
namespace web {

HistoryCache::HistoryCache(size_t max_size_)
	:	max_size(max_size_)
{
}

std::shared_ptr<const Content> HistoryCache::query(const Path& path) const {
	auto iter = content_map.find(path);
	if(iter != content_map.end()) {
		return iter->second;
	}
	return 0;
}

void HistoryCache::insert(const Path& path, std::shared_ptr<const Content> content) {
	if(max_size > 0) {
		content_map[path] = content;
		history.push(path);
	}
	while(history.size() > max_size) {
		content_map.erase(history.front());
		history.pop();
	}
}

void HistoryCache::remove(const Path& path) {
	content_map.erase(path);
}

void HistoryCache::purge() {
	content_map.clear();
}


} // web
} // vnx
