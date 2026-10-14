# C-Multithreading-Demos-Threads-OpenMP
This repository contains 2 mini project and 1 code-based demonstration. These codes are written in C++ and were used for a workshop aiming to teach the basics of OpenMP and C++ threads.

## Demo
The demo simply creates a large vector and applies 2 equivalent mock-processing tasks using for-loops. The first loop is sequential, and the second is parallelized using OpenMP.
The execution of both loops is timed and the results + speedup is printed to the console.

## Mini-Project 1
The first mini-project is a simple timing game. A random time with precision of 2 is generated, and the user is presented with a stopwatch and is tasked with pressing the enter key at the correct time. 
2 threads are utilized for this: one for displaying the stopwatch, and one for handling inputs. The main thread creates both of these using c++ threads.

## Mini-Project 2
The second mini-project creates a producer-consumer application with a single-threaded producer and a multi-threaded consumer. The consumer uses OpenMP to run consumers in parallel.
A thread-safe queue is implemented using OpenMP critical sections and is used for communication between the consumer and producer. 
The main thread creates a producer- and consumer-manager thread using C++ threads which create the producer and consumer objects and run them in parallel.
This project also uses template classes heavily and makes use of compile-time error checking to mitigate runtime errors.
