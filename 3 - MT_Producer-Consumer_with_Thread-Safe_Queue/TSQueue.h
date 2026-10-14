#pragma once

#include <iostream>
#include <stdexcept>
#include <thread>

//this queue will be implemented in TSQueue.cpp
//the queue will be implemented as a circular queue using an array
template <typename T, std::size_t size_>
class TSQueue
{
private:
	T* queue_;
	std::size_t front_;
	std::size_t back_;

public:
	//we don't want these constructors
	//they are more trouble than they are worth
	TSQueue(TSQueue&& rvR) = delete;
	TSQueue(TSQueue& lvR) = delete;
	TSQueue operator=(TSQueue&& rvR) = delete;
	TSQueue operator=(TSQueue& lvR) = delete;

	//the only constructor we want/need
	TSQueue();

	//add an element to the back of the queue, if there is space
	auto push(const T&);
	//remove an element from the front of the queue, if not empty
	auto pop();

	//destructor
	~TSQueue();
};

template <typename T, std::size_t size_>
TSQueue<T, size_>::TSQueue() {
	static_assert(size_ > 1, "TSQueue size must be greater than 1");

	queue_ = new T[size_];
	front_ = 0;
	back_ = 0;
}

template <typename T, std::size_t size_>
auto TSQueue<T, size_>::pop() {
	T r;
	bool isEmpty;

#pragma omp critical(TSQueue_lock)
	{
		//check for emptyness
		isEmpty = (front_ == back_);

		//only grab the necessary data when not empty
		if (!isEmpty) [[likely]] {
			//return the value found at the front
			r = queue_[front_];

			//update front position
			front_ = ((front_ + 1) % size_);
		}
	}

	//you cannot throw exceptions or return from inside a critical section
	if (isEmpty)
		throw std::runtime_error("Queue is empty, cannot dequeue");
	return r;
}

template <typename T, std::size_t size_>
auto TSQueue<T, size_>::push(const T& element) {
	bool isFull;

#pragma omp critical(TSQueue_lock)
	{
		//check for fullness
		isFull = (((back_ + 1) % size_) == (front_));

		if (!isFull) [[likely]]
		{
			//store the element in the queue, if it is not full
			queue_[back_] = element;

			//update the back position
			back_ = ((back_ + 1) % size_);
		}
	}

	if (isFull)
		throw std::runtime_error("Queue is full, cannot enqueue");
	return;
}

template <typename T, std::size_t size_>
TSQueue<T, size_>::~TSQueue() {
	delete[] queue_;
	queue_ = nullptr;
}