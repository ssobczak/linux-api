/*
 *  Created on: Feb 22, 2013
 *      Author: ssobczak
 */

#ifndef WORKERSPOOL_H_
#define WORKERSPOOL_H_

#include <condition_variable>
#include <mutex>

#include "eventqueue.h"

class WorkersPool {
public:
	typedef EventQueue::callback job;

	WorkersPool();
	virtual ~WorkersPool();

	void add_job(job& job);

private:
	std::mutex mutex_;
	std::condition_variable cond_;

	EventQueue job_q_;
};

#endif /* WORKERSPOOL_H_ */

