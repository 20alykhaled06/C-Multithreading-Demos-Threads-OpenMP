#include <iostream>
#include <atomic>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <chrono>

//uses the appropriate system command to clear the console
//prints a new line if not windows or linux
//in class, it is enough to give the correct one for the student's OS.
#ifdef _WIN32
#define CLEARSCREEN system("cls")
#elifdef __linux__
#define CLEARSCREEN system("clear")
#else
#define CLEARSCREEN std::cout << "\n\n\n\n"
#endif

//global atomic variables
//these are atomic because they are touched by multiple threads
//them being atomic prevents race conditions
std::atomic<int> targetTime;
std::atomic<int> passedTime;
std::atomic<bool> keyPressed = false;

auto displayClock() {
	using namespace std::chrono_literals;

	passedTime = 0;

	while (!keyPressed) {
		CLEARSCREEN;

		//update passedTime BEFORE printing, so when keyPressed becomes true, it reacts to the displayed time
		passedTime += 100;

		//display
		std::cout << "Target time: " << targetTime / 1000 << "." << targetTime % 1000 << " seconds.\n\n";
		std::cout << passedTime / 1000 << "." << passedTime % 1000;
		
		//sleep for 0.1 seconds, otherwise it go by way too fast
		std::this_thread::sleep_for(100ms);

		//limit to prevent clock from going on forever
		if (passedTime > 11000)
			return;
	}

	CLEARSCREEN;
	if (passedTime == targetTime) {
		std::cout << "Congratulations, you got it exactly right!!!";
	}
	else {
		std::cout << "Awww, you missed the target :(";
	}

	std::cout << "\n\nTarget time: " << targetTime / 1000 << "." << targetTime % 1000 << " seconds.\n";
	std::cout << "Your time:  " << passedTime / 1000 << "." << passedTime % 1000 << " seconds.\n";

	return;
}

//the thread that'll run this function will wait for you to hit "enter", then will set keyPressed to true
auto handleInput() {
	std::cin.get();
	keyPressed = true;
	return;
}

int main() {
	CLEARSCREEN;

	//create a random time from 1-10 with a precision of 2
	srand(time(0));
	targetTime = rand() % 91 + 10;
	targetTime = targetTime * 100;

	std::cout << "Welcome to the reaction game!!!!\n"
		<< "You will need to press \"enter\" when the timer hits:\n\n"
		<< targetTime / 1000 << "." << targetTime % 1000 << " seconds.\n\n"
		<< "Press enter to start the game!";
	std::cin.get();

	CLEARSCREEN;

	//create the threads
	std::thread counter(displayClock);
	std::thread inputHandler(handleInput);
	
	//join the threads
	//all created threads MUST be joined or detached before main returns
	counter.join();
	inputHandler.join();

	return 0;
}