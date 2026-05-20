#ifndef STACKARRAY_H
#define STACKARRAY_H

#include "Stack.h"
#include <cstddef>
#include <utility>

template<typename T>
class StackArray final : public Stack<T> {
private:
    T* data_;
    std::size_t capacity_;
    std::size_t size_;

public:
    explicit StackArray(std::size_t capacity);
    ~StackArray();

    void swap(StackArray<T>& other) noexcept;

    StackArray(const StackArray<T>& other);
    StackArray<T>& operator=(const StackArray<T>& other);
    StackArray(StackArray<T>&& other) noexcept;
    StackArray<T>& operator=(StackArray<T>&& other) noexcept;

    bool isEmpty() override;
    const T& top() override;
    T pop() override;
    void push(const T& new_element) override;

    std::size_t size() const;
    std::size_t capacity() const;
};

template<typename T>
StackArray<T>::StackArray(std::size_t capacity)
    : data_(nullptr)
    , capacity_(capacity)
    , size_(0)
{
    if (capacity_ <= 0) {
        throw WrongStackSize();
    }

    data_ = new T[capacity_ + 1];
}

template<typename T>
StackArray<T>::~StackArray() {
    delete[] data_;
}

template<typename T>
void StackArray<T>::swap(StackArray<T>& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
}

template<typename T>
StackArray<T>::StackArray(const StackArray<T>& other)
    : data_(new T[other.capacity_ + 1])
    , capacity_(other.capacity_)
    , size_(other.size_)
{
    for (std::size_t i = 1; i <= size_; ++i)
    {
        data_[i] = other.data_[i];
    }
}

template<typename T>
StackArray<T>& StackArray<T>::operator=(const StackArray<T>& other) {
    if (this != &other) {
        StackArray<T> temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
StackArray<T>::StackArray(StackArray<T>&& other) noexcept
    : data_(other.data_)
    , capacity_(other.capacity_)
    , size_(other.size_)
{
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
}

template<typename T>
StackArray<T>& StackArray<T>::operator=(StackArray<T>&& other) noexcept {
    if (this != &other) {
        StackArray<T> temp(std::move(other));
        swap(temp);
    }
    return *this;
}

template<typename T>
bool StackArray<T>::isEmpty() {
    return size_ == 0;
}

template<typename T>
const T& StackArray<T>::top() {
    if (isEmpty()) {
        throw StackUnderflow();
    }
    return data_[size_];
}

template<typename T>
T StackArray<T>::pop() {
    if (isEmpty()) {
        throw StackUnderflow();
    }
    return data_[size_--];
}

template<typename T>
void StackArray<T>::push(const T& new_element) {
    if (size_ == capacity_) {
        throw StackOverflow();
    }
    data_[++size_] = new_element;
}

template<typename T>
std::size_t StackArray<T>::size() const {
    return size_;
}

template<typename T>
std::size_t StackArray<T>::capacity() const {
    return capacity_;
}

#endif // STACKARRAY_H
