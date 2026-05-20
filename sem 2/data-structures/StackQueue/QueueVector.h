#ifndef QUEUEVECTOR_H
#define QUEUEVECTOR_H

#include "Queue.h"
#include "lib/Vector/Vector.h"
#include <cstddef>
#include <stdexcept>
#include <utility>

template<typename T>
class QueueVector final : public Queue<T> {
private:
    Vector<T> data_;
    std::size_t head_;
    std::size_t tail_;
    std::size_t current_size_;

    static constexpr std::size_t DEFAULT_CAPACITY = 10;

    void resize();

public:
    QueueVector();
    explicit QueueVector(std::size_t capacity);

    QueueVector(const QueueVector<T>& other) = default;
    QueueVector(QueueVector<T>&& other) noexcept;
    QueueVector<T>& operator=(const QueueVector<T>& other) = default;
    QueueVector<T>& operator=(QueueVector<T>&& other) noexcept;
    ~QueueVector() override = default;

    void enQueue(const T& e) override;
    T deQueue() override;
    bool isEmpty() const override;
    const T& front() const override;

    std::size_t size() const;
    std::size_t capacity() const;
};

template<typename T>
QueueVector<T>::QueueVector()
    : data_(DEFAULT_CAPACITY)
    , head_(0)
    , tail_(0)
    , current_size_(0)
{}

template<typename T>
QueueVector<T>::QueueVector(std::size_t capacity)
    : data_(capacity)
    , head_(0)
    , tail_(0)
    , current_size_(0)
{
    if (capacity == 0) {
        throw WrongQueueSize();
    }
}

template<typename T>
QueueVector<T>::QueueVector(QueueVector<T>&& other) noexcept
    : data_(std::move(other.data_))
    , head_(other.head_)
    , tail_(other.tail_)
    , current_size_(other.current_size_)
{
    other.head_ = 0;
    other.tail_ = 0;
    other.current_size_ = 0;
}

template<typename T>
QueueVector<T>& QueueVector<T>::operator=(QueueVector<T>&& other) noexcept {
    if (this != &other) {
        data_ = std::move(other.data_);
        head_ = other.head_;
        tail_ = other.tail_;
        current_size_ = other.current_size_;

        other.head_ = 0;
        other.tail_ = 0;
        other.current_size_ = 0;
    }
    return *this;
}

template<typename T>
void QueueVector<T>::enQueue(const T& e) {
    if (current_size_ == data_.capacity()) {
        resize();
    }

    data_[tail_] = e;
    tail_ = (tail_ + 1) % data_.capacity();
    current_size_++;
}

template<typename T>
void QueueVector<T>::resize() {
    std::size_t old_cap = data_.capacity();
    std::size_t new_cap = old_cap * 2;
    if (old_cap == 0) {
        new_cap = DEFAULT_CAPACITY;
    }

    Vector<T> new_data(new_cap);
    for (std::size_t i = 0; i < current_size_; ++i) {
        new_data[i] = data_[(head_ + i) % old_cap];
    }

    data_ = std::move(new_data);
    head_ = 0;
    tail_ = current_size_;
}

template<typename T>
T QueueVector<T>::deQueue() {
    if (isEmpty()) {
        throw QueueUnderflow();
    }

    T front = data_[head_];
    head_ = (head_ + 1) % data_.capacity();
    current_size_--;

    return front;
}

template<typename T>
bool QueueVector<T>::isEmpty() const {
    return current_size_ == 0;
}

template<typename T>
const T& QueueVector<T>::front() const {
    if (isEmpty()) {
        throw QueueUnderflow();
    }
    return data_[head_];
}

template<typename T>
std::size_t QueueVector<T>::size() const {
    return current_size_;
}

template<typename T>
std::size_t QueueVector<T>::capacity() const {
    return data_.capacity();
}

#endif // QUEUEVECTOR_H
