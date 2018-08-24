
#ifndef INCLUDE_VNX_WEB_INTERNALPIPE_H
#define INCLUDE_VNX_WEB_INTERNALPIPE_H

#include <mutex>
#include <vector>
#include <memory>


namespace vnx {
namespace web {

template<typename T>
class InternalPipe {
public:
	InternalPipe() {
		queue.reset(new std::vector<T>());
	}
	
	void push(const T& sample) {
		std::lock_guard<std::mutex> lock(mutex);
		queue->push_back(sample);
	}
	
	template<typename Iter>
	void insert(Iter begin, Iter end) {
		std::lock_guard<std::mutex> lock(mutex);
		queue->insert(queue->end(), begin, end);
	}
	
	std::unique_ptr<std::vector<T>> pop_all() {
		std::lock_guard<std::mutex> lock(mutex);
		std::unique_ptr<std::vector<T>> result(queue.release());
		queue.reset(new std::vector<T>());
		return result;
	}
	
private:
	std::mutex mutex;
	std::unique_ptr<std::vector<T>> queue;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_INTERNALPIPE_H
