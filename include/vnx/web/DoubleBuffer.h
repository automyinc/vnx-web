
#ifndef INCLUDE_VNX_WEB_DOUBLEBUFFER_H
#define INCLUDE_VNX_WEB_DOUBLEBUFFER_H

#include <mutex>
#include <condition_variable>


namespace vnx {
namespace web {

/*
 * Single producer, single consumer double buffer.
 */
template<typename T>
class DoubleBuffer {
public:
	T& input() {
		return data[state % 2];
	}
	
	T& output() {
		return data[(state + 1) % 2];
	}
	
	bool write_lock() {
		std::unique_lock<std::mutex> lock(mutex);
		ready = false;
		return consumed;
	}
	
	void write_unlock() {
		std::unique_lock<std::mutex> lock(mutex);
		if(!locked && consumed) {
			flip();
		} else {
			ready = true;
		}
	}
	
	const T& read_lock() {
		std::unique_lock<std::mutex> lock(mutex);
		if(ready) {
			flip();
		}
		while(!available) {
			condition.wait(lock);
		}
		locked = true;
		return output();
	}
	
	const T* try_read_lock() {
		std::unique_lock<std::mutex> lock(mutex);
		if(available) {
			locked = true;
			return &output();
		}
		return 0;
	}
	
	void read_unlock() {
		std::unique_lock<std::mutex> lock(mutex);
		locked = false;
		available = false;
		consumed = true;
	}
	
private:
	void flip() {
		state = (state + 1) % 2;
		ready = false;
		available = true;
		consumed = false;
		condition.notify_all();
	}
	
private:
	std::mutex mutex;
	std::condition_variable condition;
	
	T data[2];
	int state = 0;
	bool locked = false;
	bool ready = false;
	bool available = false;
	bool consumed = true;
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_DOUBLEBUFFER_H
