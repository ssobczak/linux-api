/*
 *  Created on: Feb 22, 2013
 *      Author: ssobczak
 */

#include "workerspool.h"

WorkersPool::WorkersPool(int workers) {
	std::unique_lock<std::mutex> lock(mutex_);

	for (int i = 0; i != workers; i++) {
		workers_.push_back(std::thread(&WorkersPool::do_job, this));
	}

	running = true;
}

WorkersPool::~WorkersPool() {
	stop();
}

void WorkersPool::stop() {
	mutex_.lock();
	running = false;
	cond_.notify_all();
	mutex_.unlock();

	for (size_t i = 0; i != workers_.size(); i++) {
		workers_[i].join();
	}
}

void WorkersPool::add_job(job& job) {
	std::unique_lock<std::mutex> lock(mutex_);

	job_q_.add(job);
	cond_.notify_one();
}

