#ifndef STACK_H
#define STACK_H

#include <exception>

template<typename T>
class Stack {
public:
    virtual ~Stack() {}
    virtual void push(const T& e) = 0;
    virtual T pop() = 0;
    virtual bool isEmpty() = 0;
    virtual const T& top() = 0;
};

class StackOverflow : public std::exception {
public:
    const char* what() const noexcept override {
        return "Stack overflow";
    }
};

class StackUnderflow : public std::exception {
public:
    const char* what() const noexcept override {
        return "Stack underflow";
    }
};

class WrongStackSize : public std::exception {
public:
    const char* what() const noexcept override {
        return "Wrong stack size";
    }
};

#endif // STACK_H
