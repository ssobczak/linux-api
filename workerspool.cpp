/*
 *  Created on: Feb 22, 2013
 *      Author: ssobczak
 */

#include "workerspool.h"

#include <thread>

WorkersPool::WorkersPool(int workers) {
	std::unique_lock<std::mutex> lock(mutex_);

	workers_.resize(workers);

	for (int i = 0; i != workers; i++) {
		workers_[i] = std::thread(&WorkersPool::do_job, this);
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

void WorkersPool::add_job(const job& job) {
	std::unique_lock<std::mutex> lock(mutex_);

	job_q_.add(job);
	cond_.notify_one();
}

void WorkersPool::do_job() {
	std::unique_lock<std::mutex> lock(mutex_);

	job job;
	while (running) {
		cond_.wait(lock);

		if (job_q_.pop(&job)) {
			job();
		}
	}
}
