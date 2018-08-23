
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
	
	const T& output() {
		return data[(state + 1) % 2];
	}
	
	bool write_lock() {
		std::lock_guard<std::mutex> lock(mutex);
		return write_counter == 0;
	}
	
	void write_unlock() {
		std::lock_guard<std::mutex> lock(mutex);
		write_counter++;
		if(!locked && read_counter == 0) {
			flip();
		}
	}
	
	const T* try_read_lock() {
		std::lock_guard<std::mutex> lock(mutex);
		if(read_counter) {
			locked = true;
			return &output();
		}
		return 0;
	}
	
	void read_unlock() {
		std::lock_guard<std::mutex> lock(mutex);
		locked = false;
		read_counter = 0;
	}
	
private:
	void flip() {
		state = (state + 1) % 2;
		const size_t tmp = write_counter;
		write_counter = read_counter;
		read_counter = tmp;
	}
	
private:
	std::mutex mutex;
//	std::condition_variable condition;
	
	T data[2] = {};
	size_t read_counter = 0;
	size_t write_counter = 0;
	int state = 0;
	bool locked = false;
	
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_DOUBLEBUFFER_H
