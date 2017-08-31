#pragma once
#include <boost/thread.hpp>
#include <deque>
#include <mutex>
static boost::mutex s_iomutex;
template <typename T> class BlockingQueue {
public:
	explicit BlockingQueue(size_t capacity) : _buffer(), _capacity(capacity) {
		assert(capacity > 0);
	}

void push(const T &elem) {
		boost::unique_lock<boost::mutex> lock(_mutex);
		_pop_event.wait(lock, [&] { return _buffer.size() < _capacity; });
		_buffer.push_back(elem);
		_push_event.notify_one(); // notifies one of the waiting threads which are blocked on the queue
								  // assert(!_buffer.empty());
	}

T pop() {
		boost::unique_lock<boost::mutex> lock(_mutex);
		_push_event.wait(lock, [&] { return _buffer.size() > 0; });
		T elem = _buffer.front();
		_buffer.pop_front();
		_pop_event.notify_one();
		return elem;
	}
BlockingQueue<T>& operator=(BlockingQueue<T> const &q)
{
	_buffer = q._buffer;
	_capacity = q._capacity;
	return *this;
}
void clear()
{
	_buffer.clear();
}
bool isEmpty()
{
	std::lock_guard<boost::mutex>lock(_mutex);
	return _buffer.empty();
}
private:
	boost::mutex _mutex;
	boost::condition_variable _push_event, _pop_event;
	std::deque<T> _buffer;
	size_t _capacity;
};

