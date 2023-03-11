##Project Title

Producer-Consumer Problem with Semaphores in C++

##Purpose

The purpose of this project is to demonstrate how to solve the classic Producer-Consumer problem using semaphores in C++. The problem involves two processes, a producer that produces data and adds it to a buffer, and a consumer that removes data from the buffer and uses it. The goal is to ensure that the producer and consumer do not access the buffer at the same time, which can lead to race conditions and other issues. Semaphores are a synchronization technique that can be used to ensure that only one process can access the buffer at a time.

##Version

This is version 1.0 of the Producer-Consumer problem with Semaphores in C++.

##How-To Guide

    ###Files:

        -first_try.cpp: This is the C++ file that contains a version of the Producer-Consumer problem that has a Producer limitation. 
            the CSCI331ProjectTwo.cpp file was updated based on this code.

        -bounded_buffer_semaphore.cpp: This is the main C++ file that contains the code for the Producer-Consumer problem with semaphores.


    ###Building and Running the CPP file:

        1.Open a terminal window and navigate to the project folder.

        2.Compile the main.cpp file using the following command:

            g++ bounded_buffer_semaphore.cpp -o main -pthread

        3.Run the executable using the following command:
            
            ./main

## User Instructions

When you run the main executable, the program will create a bounded buffer of size 10 and start a producer and a consumer thread. The producer thread will produce data and add it to the buffer, while the consumer thread will remove data from the buffer and consume it. The producer and consumer threads will synchronize using semaphores to ensure that the buffer is not accessed at the same time.

You can modify the behavior of the program by changing the sleep times in the Producer and Consumer classes. The sleep times determine how often the producer and consumer threads produce and consume data, respectively.

## Authors

This project was created by Dr_Bee.
