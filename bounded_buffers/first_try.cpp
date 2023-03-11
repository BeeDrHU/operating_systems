#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

class BoundedBuffer {
private:
    std::vector<int> buffer;
    int next_in = 0;
    int next_out = 0;
public:
    BoundedBuffer(int size) {
        buffer.resize(size);
    }
    void produce() {
        std::srand(std::time(nullptr));
        int k1 = std::rand() % buffer.size();
        for (int i = 0; i < k1; i++) {
            buffer[(next_in + i) % buffer.size()] = 1;
        }
        next_in = (next_in + k1) % buffer.size();
        int t1 = std::rand() % 1000;
        std::this_thread::sleep_for(std::chrono::milliseconds(t1));
    }
    void consume() {
        std::srand(std::time(nullptr));
        int t2 = std::rand() % 1000;
        std::this_thread::sleep_for(std::chrono::milliseconds(t2));
        int k2 = std::rand() % buffer.size();
        for (int i = 0; i < k2; i++) {
            int data = buffer[(next_out + i) % buffer.size()];
            if (data > 1) {
                std::cout << "Race condition detected: consumer too slow" << std::endl;
                std::exit(1);
            } else if (data == 0) {
                std::cout << "Race condition detected: producer too slow" << std::endl;
                std::exit(1);
            } else {
                buffer[(next_out + i) % buffer.size()] = 0;
            }
        }
        next_out = (next_out + k2) % buffer.size();
    }
};

class Producer {
private:
    BoundedBuffer* buffer;
public:
    Producer(BoundedBuffer* buffer) {
        this->buffer = buffer;
    }
    void operator() () {
        while (true) {
            buffer->produce();
        }
    }
};

class Consumer {
private:
    BoundedBuffer* buffer;
public:
    Consumer(BoundedBuffer* buffer) {
        this->buffer = buffer;
    }
    void operator() () {
        while (true) {
            buffer->consume();
        }
    }
};

int main() {
    BoundedBuffer buffer(10);
    Producer producer(&buffer);
    Consumer consumer(&buffer);
    std::thread producer_thread(producer);
    std::thread consumer_thread(consumer);
    producer_thread.join();
    consumer_thread.join();
    return 0;
}