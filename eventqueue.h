/*
 *  Created on: Feb 25, 2013
 *      Author: ssobczak
 */

#ifndef EVENTQUEUE_H_
#define EVENTQUEUE_H_

#include <functional>
#include <mutex>
#include <queue>

class EventQueue {
public:
	typedef std::function<bool (void)> callback;

	EventQueue() {}
	virtual ~EventQueue() {}

	void add(callback cb);
	bool pop(callback* cb);

	bool flush();

private:
	std::mutex mutex_;
	std::queue<callback> cb_queue_;
};

#endif /* EVENTQUEUE_H_ */
