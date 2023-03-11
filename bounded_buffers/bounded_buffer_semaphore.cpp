#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

class Semaphore {
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
public:
    Semaphore(int count) : count(count) {}
    void P() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return count > 0; });
        count--;
    }
    void V() {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        cv.notify_one();
    }
};

class BoundedBuffer {
private:
    int* buffer;
    int size;
    int in;
    int out;
    std::mutex* mutex_lock;
public:
    BoundedBuffer(int size) {
        this->size = size;
        buffer = new int[size];
        in = 0;
        out = 0;
        mutex_lock = new std::mutex();
    }
    void deposit(int data) {
        mutex_lock->lock();
        buffer[in] = data;
        in = (in + 1) % size;
        mutex_lock->unlock();
    }
    int fetch() {
        mutex_lock->lock();
        int data = buffer[out];
        out = (out + 1) % size;
        mutex_lock->unlock();
        return data;
    }
};

class Producer {
private:
    BoundedBuffer* buffer;
    Semaphore* empty;
    Semaphore* full;
public:
    Producer(BoundedBuffer* buffer, Semaphore* empty, Semaphore* full) {
        this->buffer = buffer;
        this->empty = empty;
        this->full = full;
    }
    void operator() () {
        while (true) {
            empty->P(); // wait for an empty slot
            buffer->deposit(1);
            full->V(); // signal that a slot is now full
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
};

class Consumer {
private:
    BoundedBuffer* buffer;
    Semaphore* empty;
    Semaphore* full;
public:
    Consumer(BoundedBuffer* buffer, Semaphore* empty, Semaphore* full) {
        this->buffer = buffer;
        this->empty = empty;
        this->full = full;
    }
    void operator() () {
        while (true) {
            full->P(); // wait for a full slot
            int data = buffer->fetch();
            empty->V(); // signal that a slot is now empty
            std::cout << "Consumed: " << data << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
};

int main() {
    BoundedBuffer* buffer = new BoundedBuffer(10);
    Semaphore* empty = new Semaphore(10);
    Semaphore* full = new Semaphore(0);
    Producer producer(buffer, empty, full);
    Consumer consumer(buffer, empty, full);
    std::thread producer_thread(producer);
    std::thread consumer_thread(consumer);
    producer_thread.join();
    consumer_thread.join();
    return 0;
}