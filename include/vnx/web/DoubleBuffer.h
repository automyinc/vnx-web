
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
		write_locked = true;
		return write_counter == 0;
	}
	
	void write_unlock() {
		std::lock_guard<std::mutex> lock(mutex);
		write_counter++;
		if(read_counter == 0 && !read_locked) {
			flip();
		}
		write_locked = false;
	}
	
	const T* try_read_lock() {
		std::lock_guard<std::mutex> lock(mutex);
		if(!read_counter && write_counter && !write_locked) {
			flip();
		}
		if(read_counter) {
			read_locked = true;
			return &output();
		}
		return 0;
	}
	
	void read_unlock() {
		std::lock_guard<std::mutex> lock(mutex);
		read_locked = false;
		read_counter = 0;
	}
	
private:
	void flip() {
		state = (state + 1) % 2;
		read_counter = write_counter;
		write_counter = 0;
	}
	
private:
	std::mutex mutex;
//	std::condition_variable condition;
	
	T data[2] = {};
	size_t read_counter = 0;
	size_t write_counter = 0;
	int state = 0;
	bool read_locked = false;
	bool write_locked = false;
	
	
};


} // web
} // vnx

#endif // INCLUDE_VNX_WEB_DOUBLEBUFFER_H
