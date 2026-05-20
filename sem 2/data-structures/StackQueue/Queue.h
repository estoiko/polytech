#ifndef QUEUE_H
#define QUEUE_H

#include <exception>

template <typename T>
class Queue {
public:
    virtual ~Queue() {}
    virtual void enQueue(const T& e) = 0;
    virtual T deQueue() = 0;
    virtual bool isEmpty() const = 0;
    virtual const T& front() const = 0;
};

class QueueUnderflow : public std::exception {
public:
    const char* what() const noexcept override {
        return "Queue underflow";
    }
};

class WrongQueueSize : public std::exception {
public:
    const char* what() const noexcept override {
        return "Wrong queue size";
    }
};

#endif // QUEUE_H
