/*
 *  Created on: Feb 22, 2013
 *      Author: ssobczak
 */

#ifndef WORKERSPOOL_H_
#define WORKERSPOOL_H_

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

#include "eventqueue.h"

class WorkersPool {
public:
	typedef EventQueue::callback job;

	WorkersPool(int workers);
	virtual ~WorkersPool();

	void add_job(job& job);
	void stop();

private:
	std::mutex mutex_;
	bool running;

	std::condition_variable cond_;

	EventQueue job_q_;
	std::vector<std::thread> workers_;

	void do_job() {
		std::unique_lock<std::mutex> lock(mutex_);

		job job;
		while (!job_q_.pop(&job)) {
			cond_.wait(lock);

			if (!running) {
				return;
			}

			job();
		}
	}
};

#endif /* WORKERSPOOL_H_ */

