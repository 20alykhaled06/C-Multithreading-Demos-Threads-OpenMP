#pragma once

#include <chrono>

#include "TSQueue.h"

using namespace std::chrono;

// This is a server which processes integer inputs in parallel
// Constructed with:
// 1. A TSQueue<int, std::size_t> lvalue reference from which to read data
// 2. [How long it takes, in milliseconds, and on average, for the server to process an integer]
// 3. [The total time, in milliseconds, the server will run for (note: the server will not interrupt processing to terminate)]
// 4. [The number of threads used for processing, 4 by default]
template <std::size_t s>
class integerServer
{
private:
	TSQueue<int, s>& queue_;
	uint8_t threads_; //up to 255 threads
	milliseconds processingTime_; //for one integer
	milliseconds totalTime_;
public:
	integerServer() = delete;
	integerServer(integerServer&) = delete;
	integerServer(integerServer&&) = delete;
	integerServer operator=(integerServer&) = delete;
	integerServer operator=(integerServer&&) = delete;

	integerServer(TSQueue<int, s>& queue, milliseconds periodMS = milliseconds(100), 
		milliseconds totalTime = milliseconds(10000), uint8_t threadsNum = 4)
		: processingTime_(periodMS), queue_(queue), 
		  threads_(threadsNum), totalTime_(totalTime) {}

	auto serve() 
	{
		using namespace std::literals::chrono_literals;
		
		auto start = high_resolution_clock::now();
		auto end = high_resolution_clock::now();

		milliseconds currentTime = duration_cast<milliseconds>(end - start);

		#pragma omp parallel num_threads(threads_) 
		{
			for (; currentTime < totalTime_;
				end = high_resolution_clock::now(),
				currentTime = duration_cast<milliseconds>(end - start))
			{
				try {
					auto data = queue_.pop();
					//"processing" with some random cost
					std::this_thread::sleep_for(
						(processingTime_) + ((((rand() % 51) / 100) - 0.25) 
							* (processingTime_))
					);
					//printing output
					#pragma omp critical(consoleOutput)
					{
						std::cout << "\t\t\t\t\t\t\tThread #" << std::this_thread::get_id()
							<< ": processed:" << data << std::endl;
					}
				}
				catch (const std::exception& e) {
					#pragma omp critical(consoleOutput)
					{
						std::cout << "\t\t\t\t\t\t\tError from thread " << std::this_thread::get_id()
							<< ": " << e.what() << std::endl;
					}
				//if the queue is empty, sleep for a bit and try again
				std::this_thread::sleep_for(processingTime_);
				}
			}
		}

		return;
	}
};