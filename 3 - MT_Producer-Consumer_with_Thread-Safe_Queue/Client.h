#pragma once

#include <cstdlib>
#include <chrono>
#include "TSQueue.h"

using namespace std::chrono;

// This class generates random values of type T and puts them in the queue
// Constructed with:
// 1. A TSQueue lvalue reference from which to read data
// 2. [A seed for the random generation]
// 3. [How long it takes, in milliseconds, for the client to generate a value]
// 4. [The total time, in milliseconds, the server will run for (note: the client will not interrupt generation to terminate)]
template <typename T, std::size_t s>
class Client
{
private:
	TSQueue<T, s>& queue_;
	milliseconds period_;
	milliseconds totalTime_;
public:
	Client() = delete;
	Client(Client&) = delete;
	Client(Client&&) = delete;
	Client operator=(Client&) = delete;
	Client operator=(Client&&) = delete;

	//init with a seed for random generation, a period, a lifetime, and a queue reference
	Client(TSQueue<T, s>& queue, unsigned int seed = 0, milliseconds periodMS = milliseconds(100), milliseconds totalTime = milliseconds(10000))
		: period_(periodMS), queue_(queue), totalTime_(totalTime)
	{
		srand(seed);
	}

	auto work()
	{
		auto start = high_resolution_clock::now();
		auto end = high_resolution_clock::now();

		milliseconds currentTime = duration_cast<milliseconds>(end - start);
		
		for (; currentTime < totalTime_;
			end = high_resolution_clock::now(),
			currentTime = duration_cast<milliseconds>(end - start))
		{
			try { 
				T temp(rand() % 100);
				queue_.push(temp);
				#pragma omp critical(consoleOutput)
				{
					std::cout << "Thread #" << std::this_thread::get_id()
						<< ": created:" << temp << std::endl;
				}
			}
			catch (const std::exception& e) {
				#pragma omp critical(consoleOutput)
				{
					std::cout << "Error from thread " << std::this_thread::get_id() << ": "
						<< e.what() << std::endl;
				}
			}
			std::this_thread::sleep_for(period_);
		}
	}
};