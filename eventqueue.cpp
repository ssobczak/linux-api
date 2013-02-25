/*
 *  Created on: Feb 25, 2013
 *      Author: ssobczak
 */

#include <mutex>

#include "eventqueue.h"

void EventQueue::add(callback cb) {
	std::lock_guard<std::mutex> guard(mutex_);

	cb_queue_.emplace(cb);
}

bool EventQueue::pop(callback* result) {
	std::lock_guard<std::mutex> guard(mutex_);

	if (cb_queue_.empty()) {
		return false;
	}

	*result = cb_queue_.front();
	cb_queue_.pop();
	return true;
}

bool EventQueue::flush() {
	std::lock_guard<std::mutex> guard(mutex_);

	bool success = false;
	while (!cb_queue_.empty()) {
		if (!cb_queue_.front()()) {
			success = false;
		}
		cb_queue_.pop();
	}

	return success;
}
