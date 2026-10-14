//this project is for demonstration purposes only

#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

#define CURRENTTIME std::chrono::high_resolution_clock::now()
//play with the below values and see how they impact performance on your machine
//better to use constexpr rather than #defines, but oh well
#define NTHREADS 16	
#define DSIZE 10000000
/* note: 
- As NTHREADS increases, performance also increases, but not linearly.
  There is an overhead to managing each thread, which leads to diminishing returns
  as you increase the number of threads.

- For low NTHREADS and DSIZE values, the speedup may vary a lot from run to run.
  This is because the OS does not always run your machine at full power.
  It depends on how heavy the load is and also other factors like the 
  processes running in the background.

- At low values of NTHREADS, you may find super-linear speedup (i.e. doubling
  the thread cound more than doubles the performance). This is due to caching effects,
  which you are taught during COE251/241 and in more detail in COE341
*/

int main() {
	//create a big block of data
	std::vector<int> data(DSIZE);

	//populate data vector with pseudo-random numbers
	for (auto& d : data) {
		d = rand() % 10;
	}


	//sequential processing + timing
	auto seqStartTime = CURRENTTIME;
	for (auto i = 0; i < data.size(); i++) {
		data[i] *= 2;
 		data[i] *= 3;
		data[i] /= 8;
	}
	auto seqEndTime = CURRENTTIME;


	//parallel processing + timing
	auto parStartTime = CURRENTTIME;
	//same processing, but in parallel this time
#pragma omp parallel for num_threads(NTHREADS)
	for (auto i = 0; i < data.size(); i++) {
		data[i] *= 2;
		data[i] *= 3;
		data[i] /= 8;
	}
	auto parEndTime = CURRENTTIME;


	auto seqRunTime = std::chrono::duration_cast<std::chrono::microseconds>(seqEndTime - seqStartTime);
	auto parRunTime = std::chrono::duration_cast<std::chrono::microseconds>(parEndTime - parStartTime);

	//printing results:
	std::cout << "Sequential processing took: " << seqRunTime << std::endl;
	std::cout << "Parallel processing took:   "	<< parRunTime << std::endl;
	std::cout << "\nSpeedup = " << double(seqRunTime.count()) / double(parRunTime.count());
}