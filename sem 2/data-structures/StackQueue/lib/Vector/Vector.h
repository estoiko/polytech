#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <new>

template<typename T>
class Vector {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;

    static void memDelete(std::size_t from, std::size_t size, T* data);
    void memInit(std::size_t size, const T* value = nullptr);

    template <typename K>
    void add_element(K&& value);

    template<typename K>
    void insert_element(std::size_t index, K&& value);

public:
    Vector() noexcept;
    explicit Vector(std::size_t n);
    Vector(std::size_t n, const T& value);
    ~Vector() noexcept;

    void swap(Vector& other) noexcept;

    Vector(const Vector& other);
    Vector& operator=(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector& operator=(Vector&& other) noexcept;

    void reserve(std::size_t new_capacity);
    void resize(std::size_t new_size);
    void resize(std::size_t new_size, const T& value);
    void shrink_to_fit();

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back() noexcept;

    void insert(std::size_t index, const T& value);
    void insert(std::size_t index, T&& value);
    void erase(std::size_t first, std::size_t last);
    void erase(size_t index);

    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
    bool operator<(const Vector& other) const;
    bool operator>(const Vector& other) const;
    bool operator<=(const Vector& other) const;
    bool operator>=(const Vector& other) const;

    T& operator[](std::size_t index) noexcept;
    const T& operator[](std::size_t index) const noexcept;
    T& at(std::size_t index);
    const T& at(std::size_t index) const;

    void clear() noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
    bool empty() const noexcept;
    T& front() noexcept;
    const T& front() const noexcept;
    T& back() noexcept;
    const T& back() const noexcept;
};

template<typename T>
void Vector<T>::memDelete(std::size_t from, std::size_t size, T* data) {
    for (std::size_t i = from; i < size; ++i) {
        data[i].~T();
    }
}

template<typename T>
void Vector<T>::memInit(std::size_t size, const T* value) {
    if (size == 0) return;
    T* new_data = static_cast<T*>(::operator new(size * sizeof(T)));

    std::size_t i = 0;
    try {
        for (; i < size; ++i) {
            if (value) {
                new(new_data + i) T(*value);
            } else {
                new(new_data + i) T();
            }
        }
    } catch(...) {
        memDelete(0, i, new_data);
        ::operator delete(new_data);
        throw;
    }

    data_ = new_data;
    size_ = size;
    capacity_ = size;
}

template<typename T>
template <typename K>
void Vector<T>::add_element(K&& value) {
    if (size_ == capacity_) {
        reserve(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    new (data_ + size_) T(std::forward<K>(value));
    ++size_;
}

template<typename T>
template<typename K>
void Vector<T>::insert_element(std::size_t index, K&& value) {
    if (index > size_) {
        throw std::out_of_range("ERROR: Index out of range");
    }

    if (index == size_) {
        add_element(std::forward<K>(value));
        return;
    }

    if (size_ == capacity_) {
        std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;

        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

        std::size_t i = 0;
        try {
            for (; i < index; ++i) {
                new (new_data + i) T(std::move_if_noexcept(data_[i]));
            }

            new (new_data + index) T(std::forward<K>(value));

            for (std::size_t j = index; j < size_; ++j) {
                new(new_data + j + 1) T(std::move_if_noexcept(data_[j]));
            }
        } catch(...) {
            memDelete(0, i + 1, new_data);
            ::operator delete(new_data);
            throw;
        }

        memDelete(0, size_, data_);
        ::operator delete(data_);

        data_ = new_data;
        ++size_;
        capacity_ = new_capacity;
    } else {
        new (data_ + size_) T(std::move(data_[size_ - 1]));
        ++size_;

        try {
            for (std::size_t i = size_ - 2; i > index; --i) {
                data_[i] = std::move(data_[i - 1]);
            }
            data_[index] = std::forward<K>(value);
        } catch (...) {
            data_[size_ - 1].~T();
            --size_;
            throw;
        }
    }
}

template<typename T>
Vector<T>::Vector() noexcept
    : data_(nullptr)
    , size_(0)
    , capacity_(0)
{}

template<typename T>
Vector<T>::Vector(std::size_t n)
    : data_(nullptr)
    , size_(0)
    , capacity_(0)
{
    memInit(n);
}

template<typename T>
Vector<T>::Vector(std::size_t n, const T& value)
    : data_(nullptr)
    , size_(0)
    , capacity_(0)
{
    memInit(n, &value);
}

template<typename T>
Vector<T>::~Vector() noexcept{
    memDelete(0, size_, data_);
    ::operator delete(data_);
}

template<typename T>
Vector<T>::Vector(const Vector& other): Vector() {
    if (other.empty()) return;
    T* new_data = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));

    std::size_t i = 0;
    try {
        for (; i < other.size_; ++i) {
            new (new_data + i) T(other.data_[i]);
        }
    } catch (...) {
        memDelete(0, i, new_data);
        ::operator delete(new_data);
        throw;
    }

    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        Vector temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : data_(other.data_)
    , size_(other.size_)
    , capacity_(other.capacity_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        Vector temp(std::move(other));
        swap(temp);
    }
    return *this;
}

template<typename T>
void Vector<T>::reserve(std::size_t new_capacity) {
    if (new_capacity <= capacity_) return;
    T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

    std::size_t i = 0;
    try {
        for (; i < size_; ++i) {
            new(new_data + i) T(std::move_if_noexcept(data_[i]));
        }
    } catch(...) {
        memDelete(0, i, new_data);
        ::operator delete(new_data);
        throw;
    }

    memDelete(0, i, data_);
    ::operator delete(data_);

    data_ = new_data;
    capacity_ = new_capacity;
}

template<typename T>
void Vector<T>::resize(std::size_t new_size) {
    resize(new_size, T());
}

template<typename T>
void Vector<T>::resize(std::size_t new_size, const T& value) {
    if (size_ == new_size) return;
    if (new_size > capacity_) {
        reserve(new_size);
    }

    if (new_size > size_) {
        std::size_t i = size_;
        try {
            for (; i < new_size; ++i) {
                new (data_ + i) T(value);
            }
        } catch (...) {
            memDelete(size_, i, data_);
            throw;
        }
    } else {
        memDelete(new_size, size_, data_);
    }

    size_ = new_size;
}

template<typename T>
void Vector<T>::shrink_to_fit() {
    if (size_ == capacity_) return;
    if (size_ == 0) {
        ::operator delete(data_);
        data_ = nullptr;
        capacity_ = 0;
        return;
    }

    T* new_data = static_cast<T*>(::operator new(size_ * sizeof(T)));

    std::size_t i = 0;
    try {
        for (; i < size_; ++i) {
            new (new_data + i) T(std::move_if_noexcept(data_[i]));
        }
    } catch(...) {
        memDelete(0, i, new_data);
        ::operator delete(new_data);
        throw;
    }

    memDelete(0, size_, data_);
    ::operator delete(data_);

    data_ = new_data;
    capacity_ = size_;
}

template<typename T>
void Vector<T>::push_back(const T& value) {
    add_element(value);
}

template<typename T>
void Vector<T>::push_back(T&& value) {
    add_element(std::move(value));
}

template<typename T>
void Vector<T>::pop_back() noexcept {
    if (size_ > 0) data_[--size_].~T();
}

template<typename T>
void Vector<T>::insert(std::size_t index, const T& value) {
    insert_element(index, value);
}

template<typename T>
void Vector<T>::insert(std::size_t index, T&& value) {
    insert_element(index, std::move(value));
}

template<typename T>
void Vector<T>::erase(std::size_t first, std::size_t last) {
    if (first > last || last > size_) {
        throw std::out_of_range("ERROR: invalid range");
    }
    if (first == last) return;

    std::size_t diff = last - first;

    for (std::size_t i = last; i < size_; ++i) {
        data_[i - diff] = std::move(data_[i]);
    }
    memDelete(size_ - diff, size_, data_);

    size_ -= diff;
}

template<typename T>
void Vector<T>::erase(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("ERROR: index out of range");
    }

    erase(index, index + 1);
}

template<typename T>
bool Vector<T>::operator==(const Vector& other) const {
    if (size_ != other.size_) {
        return false;
    }

    for (std::size_t i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
bool Vector<T>::operator!=(const Vector& other) const {
    return !(*this == other);
}

template<typename T>
bool Vector<T>::operator<(const Vector& other) const {
    std::size_t min_size = size_ < other.size_ ? size_ : other.size();

    for (std::size_t i = 0; i < min_size; ++i) {
        if (data_[i] > other.data_[i]) {
            return false;
        }
        if (data_[i] < other.data_[i]) {
            return true;
        }
    }

    return size_ < other.size_;
}

template<typename T>
bool Vector<T>::operator>(const Vector& other) const {
    return other < *this;
}

template<typename T>
bool Vector<T>::operator<=(const Vector& other) const {
    return !(other < *this);
}

template<typename T>
bool Vector<T>::operator>=(const Vector& other) const {
    return !(*this < other);
}

template<typename T>
T& Vector<T>::operator[](std::size_t index) noexcept {
    return data_[index];
}

template<typename T>
const T& Vector<T>::operator[](std::size_t index) const noexcept {
    return data_[index];
}

template<typename T>
T& Vector<T>::at(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("ERROR: Index out of range");
    }
    return data_[index];
}

template<typename T>
const T& Vector<T>::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("ERROR: Index out of range");
    }
    return data_[index];
}

template<typename T>
void Vector<T>::clear() noexcept {
    memDelete(0, size_, data_);
    size_ = 0;
}

template<typename T>
size_t Vector<T>::size() const noexcept {
    return size_;
}

template<typename T>
size_t Vector<T>::capacity() const noexcept {
    return capacity_;
}

template<typename T>
bool Vector<T>::empty() const noexcept {
    return size_ == 0;
}

template<typename T>
T& Vector<T>::front() noexcept {
    return data_[0];
}

template<typename T>
const T& Vector<T>::front() const noexcept {
    return data_[0];
}

template<typename T>
T& Vector<T>::back() noexcept {
    return data_[size_ - 1];
}

template<typename T>
const T& Vector<T>::back() const noexcept {
    return data_[size_ - 1];
}

template<typename T>
void Vector<T>::swap(Vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename T>
void selectionSort(Vector<T>& a) {
    if (a.size() <= 1) return;

    for (std::size_t i = 0; i < a.size() - 1; ++i) {
        std::size_t min_index = i;
        for (std::size_t j = i + 1; j < a.size(); ++j) {
            if (a[j] < a[min_index]) {
                min_index = j;
            }
        }
        std::swap(a[i], a[min_index]);
    }
}

#endif // VECTOR_H
