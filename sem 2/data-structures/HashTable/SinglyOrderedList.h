#ifndef SOL_H
#define SOL_H

#include <cstddef>
#include <new>
#include <ostream>
#include <utility>
#include <stdexcept>
#include <iosfwd>

template<typename T>
class SinglyOrderedList {
private:
    struct Node {
        T data_;
        Node* next_;

        explicit Node(const T& value)
            : data_(value)
            , next_(nullptr)
        {}

        explicit Node(T&& value)
            : data_(std::move(value))
            , next_(nullptr)
        {}
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

    template<typename U>
    bool insert_element(U&& value);

public:
    SinglyOrderedList()
        : head_(nullptr)
        , tail_(nullptr)
        , size_(0)
    {}

    explicit SinglyOrderedList(const T& value)
        : head_(new Node(value))
        , tail_(head_)
        , size_(1)
    {}

    void swap(SinglyOrderedList& other) noexcept;

    SinglyOrderedList(const SinglyOrderedList& other);
    SinglyOrderedList(SinglyOrderedList&& other) noexcept;
    SinglyOrderedList& operator=(const SinglyOrderedList& other);
    SinglyOrderedList& operator=(SinglyOrderedList&& other) noexcept;

    ~SinglyOrderedList() {
        clear();
    }

    bool search(const T& key) const;

    bool insert(const T& value) {
        return insert_element(value);
    }

    bool insert(T&& value) {
        return insert_element(std::move(value));
    }

    void removeHead();
    void removeKey(const T& key);
    void remove(const SinglyOrderedList& other);

    void clear();

    const T& head() const {
        if (!head_) throw std::runtime_error("ERROR: List is empty");
        return head_->data_;
    }

    const T& tail() const {
        if (!tail_) throw std::runtime_error("ERROR: List is empty");
        return tail_->data_;
    }

    size_t size() const {
        return size_;
    }

    bool isEmpty() const {
        return size_ == 0;
    }

    void print(std::ostream& stream) const;

    bool operator==(const SinglyOrderedList& other) const;

    template<typename U>
    friend SinglyOrderedList<U> getIntersection(const SinglyOrderedList<U>& a,
                                                const SinglyOrderedList<U>& b);

    T* findPtr(const T& key);
    const T* findPtr(const T& key) const;
};

// O(n)
template<typename T>
template<typename U>
bool SinglyOrderedList<T>::insert_element(U&& value) {
    Node* new_node = new Node(std::forward<U>(value));

    if (!head_ || new_node->data_ < head_->data_) {
        new_node->next_ = head_;
        head_ = new_node;
        if (!tail_) tail_ = head_;
    } else if (new_node->data_ > tail_->data_) {
        tail_->next_ = new_node;
        tail_ = new_node;
    } else {
        Node* current = head_;
        while (current->next_ && current->next_->data_ < new_node->data_) {
            current = current->next_;
        }

        if (current->data_ == new_node->data_ ||
           (current->next_ && current->next_->data_ == new_node->data_)) {
            delete new_node;
            return false;
        }

        new_node->next_ = current->next_;
        current->next_ = new_node;
        if (!new_node->next_) tail_ = new_node;
    }
    ++size_;
    return true;
}

template<typename T>
void SinglyOrderedList<T>::swap(SinglyOrderedList& other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
}

template<typename T>
SinglyOrderedList<T>::SinglyOrderedList(const SinglyOrderedList& other)
    : head_(nullptr)
    , tail_(nullptr)
    , size_(0)
{
    try {
        Node* current = other.head_;
        while (current) {
            Node* new_node = new Node(current->data_);

            if (!head_) {
                head_ = new_node;
                tail_ = new_node;
            } else {
                tail_->next_ = new_node;
                tail_ = new_node;
            }
            ++size_;

            current = current->next_;
        }
    } catch (...) {
        clear();
        throw;
    }
}

template<typename T>
SinglyOrderedList<T>::SinglyOrderedList(SinglyOrderedList&& other) noexcept
    : head_(other.head_)
    , tail_(other.tail_)
    , size_(other.size_)
{
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
}

template<typename T>
SinglyOrderedList<T>& SinglyOrderedList<T>::operator=(const SinglyOrderedList& other) {
    if (this != &other) {
        SinglyOrderedList temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
SinglyOrderedList<T>& SinglyOrderedList<T>::operator=(SinglyOrderedList&& other) noexcept {
    if (this != &other) {
        SinglyOrderedList temp(std::move(other));
        swap(temp);
    }
    return *this;
}

// O(n)
template<typename T>
bool SinglyOrderedList<T>::search(const T& key) const {
    if (isEmpty() || head_->data_ > key || tail_->data_ < key) {
        return false;
    }

    Node* current = head_;
    while (current) {
        if (current->data_ == key) return true;
        if (current->data_ > key) return false;
        current = current->next_;
    }
    return false;
}

// O(1)
template<typename T>
void SinglyOrderedList<T>::removeHead() {
    if (isEmpty()) {
        throw std::runtime_error("ERROR: List is empty");
    }

    Node* old_head = head_;
    head_ = head_->next_;
    if (!head_) tail_ = nullptr;
    delete old_head;
    --size_;
}

// O(n)
template<typename T>
void SinglyOrderedList<T>::removeKey(const T& key) {
    if (isEmpty()) {
        throw std::runtime_error("ERROR: List is empty");
    }

    if (head_->data_ == key) {
        removeHead();
        return;
    }

    if (head_->data_ > key || tail_->data_ < key) {
        return;
    }

    Node* prev = head_;
    Node* current = head_->next_;

    while (current) {
        if (current->data_ > key) break;
        if (current->data_ == key) {
            prev->next_ = current->next_;
            if (current == tail_) tail_ = prev;
            delete current;
            --size_;
            return;
        }
        prev = current;
        current = current->next_;
    }
}

// O(n + m)
template<typename T>
void SinglyOrderedList<T>::remove(const SinglyOrderedList& other) {
    if (other.isEmpty() || this->isEmpty()) return;

    if (other.head_->data_ > tail_->data_
     || other.tail_->data_ < head_->data_) return;

    Node* prev = nullptr;
    Node* current = head_;
    Node* mask = other.head_;

    while (current && mask) {
        if (current->data_ < mask->data_) {
            prev = current;
            current = current->next_;
        } else if (current->data_ > mask->data_) {
            mask = mask->next_;
        } else {
            Node* to_remove = current;
            current = current->next_;

            if (prev) prev->next_ = current;
            else head_ = current;

            if (to_remove == tail_) tail_ = prev;

            delete to_remove;
            --size_;
        }
    }
}

// O(n)
template<typename T>
void SinglyOrderedList<T>::clear() {
    while (head_) {
        Node* next = head_->next_;
        delete head_;
        head_ = next;
    }
    tail_ = nullptr;
    size_ = 0;
}

template<typename T>
void SinglyOrderedList<T>::print(std::ostream& stream) const {
    Node* current = head_;
    while (current) {
        stream << current->data_ << " ";
        current = current->next_;
    }
    stream << "\n";
}

template<typename T>
bool SinglyOrderedList<T>::operator==(const SinglyOrderedList& other) const {
    if (size_ != other.size_) return false;

    Node* current = head_;
    Node* other_current = other.head_;

    while (current) {
        if (current->data_ != other_current->data_) {
            return false;
        }
        current = current->next_;
        other_current = other_current->next_;
    }

    return true;
}

// O(n + m)
template<typename U>
SinglyOrderedList<U> getIntersection(const SinglyOrderedList<U>& a,
                                            const SinglyOrderedList<U>& b) {
    SinglyOrderedList<U> new_list{};

    typename  SinglyOrderedList<U>::Node* a_ptr = a.head_;
    typename  SinglyOrderedList<U>::Node* b_ptr = b.head_;

    while (a_ptr && b_ptr) {
        if (a_ptr->data_ == b_ptr->data_) {
            new_list.insert(a_ptr->data_);
            a_ptr = a_ptr->next_;
            b_ptr = b_ptr->next_;
        } else if (a_ptr->data_ < b_ptr->data_) {
            a_ptr = a_ptr->next_;
        } else {
            b_ptr = b_ptr->next_;
        }
    }

    return new_list;
}

template<typename T>
T* SinglyOrderedList<T>::findPtr(const T& key) {
    Node* current = head_;
    while (current) {
        if (current->data_ == key) return &current->data_;
        if (current->data_ > key)  return nullptr;
        current = current->next_;
    }
    return nullptr;
}

template<typename T>
const T* SinglyOrderedList<T>::findPtr(const T& key) const {
    Node* current = head_;
    while (current) {
        if (current->data_ == key) return &current->data_;
        if (current->data_ > key)  return nullptr;
        current = current->next_;
    }
    return nullptr;
}

#endif // SOL_H
