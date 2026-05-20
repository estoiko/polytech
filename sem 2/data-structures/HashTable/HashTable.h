#ifndef HASH_TABLE_H
#define HASH_TABLE_H

// variant 2 - строка в стиле С - метод цепочек, определить число коллизий

#include <cstddef>
#include <cstring>
#include <ostream>
#include <utility>
#include <stdexcept>

#include "SinglyOrderedList.h"

template<typename T>
struct KeyValue {
    char* key_;
    T value_;

    KeyValue(const char* key, const T& value)
        : value_(value)
    {
        key_ = new char[std::strlen(key) + 1];
        std::strcpy(key_, key);
    }

    KeyValue(const char* key, T&& value)
        : value_(std::move(value))
    {
        key_ = new char[std::strlen(key) + 1];
        std::strcpy(key_, key);
    }

    explicit KeyValue(const char* key)
        : value_()
    {
        key_ = new char[std::strlen(key) + 1];
        std::strcpy(key_, key);
    }

    KeyValue(const KeyValue& other)
        : value_(other.value_)
    {
        key_ = new char[std::strlen(other.key_) + 1];
        std::strcpy(key_, other.key_);
    }

    KeyValue(KeyValue&& other) noexcept
        : key_(other.key_)
        , value_(std::move(other.value_))
    {
        other.key_ = nullptr;
    }

    void swap(KeyValue& other) noexcept {
        std::swap(key_, other.key_);
        std::swap(value_, other.value_);
    }

    KeyValue& operator=(const KeyValue& other) {
        if (this != &other) {
            KeyValue tmp(other);
            swap(tmp);
        }
        return *this;
    }

    KeyValue& operator=(KeyValue&& other) noexcept {
        if (this != &other) {
            KeyValue tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    ~KeyValue() { delete[] key_; }

    bool operator==(const KeyValue& other) const {
        return std::strcmp(key_, other.key_) == 0;
    }

    bool operator<(const KeyValue& other) const {
        return std::strcmp(key_, other.key_) < 0;
    }

    bool operator>(const KeyValue& other) const {
        return std::strcmp(key_, other.key_) > 0;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const KeyValue<T>& r) {
    return out << "('" <<  r.key_ << "'" << " : " << r.value_ << ")";
}

template<typename T>
class HashTable {
private:
    using List = SinglyOrderedList<KeyValue<T>>;
    std::size_t size_;
    std::size_t number_;
    std::size_t collisions_;
    List** table_;

    std::size_t hash(const char* key) const {
        if (key == nullptr) {
            throw std::invalid_argument("ERROR: key must not be nullptr");
        }

        const std::size_t p = 131;
        std::size_t h = 0;

        for (std::size_t i = 0; key[i] != '\0'; ++i) {
            unsigned char c = static_cast<unsigned char>(key[i]);
            h = (h * p + c) % (size_);
        }

        return h;
    }

public:
    explicit HashTable(std::size_t size)
        : size_(size)
        , number_(0)
        , collisions_(0)
        , table_(nullptr)
    {
        if (size == 0) {
            throw std::invalid_argument("ERROR: HashTable size must be > 0");
        }

        table_ = new List* [size]();
    }

    ~HashTable() {
        for (std::size_t i = 0; i < size_; ++i) {
            delete table_[i];
        }
        delete[] table_;
    }

    HashTable(const HashTable& other) = delete;
    HashTable& operator=(const HashTable& other) = delete;

    void swap(HashTable& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(number_, other.number_);
        std::swap(collisions_, other.collisions_);
        std::swap(table_, other.table_);
    }

    HashTable(HashTable&& other) noexcept
        : size_(other.size_)
        , number_(other.number_)
        , collisions_(other.collisions_)
        , table_(other.table_)
    {
        other.size_ = 0;
        other.number_ = 0;
        other.collisions_ = 0;
        other.table_ = nullptr;
    }

    HashTable& operator=(HashTable&& other) noexcept {
        if (this != &other) {
            HashTable tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    std::size_t getHash(const char* key) const {
        return hash(key);
    }

    void insert(const char* key, const T& value) {
        if (key == nullptr) {
            throw std::invalid_argument("ERROR: key must not be nullptr");
        }

        const std::size_t index = hash(key);

        if (!table_[index]) {
            table_[index] = new List();
        }

        const bool bucket_was_non_empty = !table_[index]->isEmpty();

        if (!(table_[index]->insert(KeyValue<T>{ key, value }))) {
            return;
        }

        if (bucket_was_non_empty) {
            ++collisions_;
        }

        ++number_;
    }

    bool contains(const char* key) const {
        return search(key) != nullptr;
    }

    const T* search(const char* key) const {
        if (key == nullptr) {
            return nullptr;
        }

        const std::size_t index = hash(key);

        if (!table_[index]) {
            return nullptr;
        }

        KeyValue<T> tmp { key };
        const KeyValue<T>* rec = table_[index]->findPtr(tmp);
        return (rec != nullptr) ? &rec->value_ : nullptr;
    }

    bool remove(const char* key) {
        if (key == nullptr) {
            return false;
        }

        const std::size_t index = hash(key);

        if (!table_[index]) return false;

        const std::size_t old_size = table_[index]->size();
        KeyValue<T> tmp { key };
        table_[index]->removeKey(tmp);

        if (table_[index]->size() < old_size) {
            --number_;

            if (table_[index]->isEmpty()) {
                delete table_[index];
                table_[index] = nullptr;
            }

            return true;
        }

        return false;
    }

    void print(std::ostream& out) const {
        for (std::size_t i = 0; i < size_; ++i) {
            out << "[" << i << "]: ";
            if (table_[i]) {
                table_[i]->print(out);
            } else {
                out << "empty\n";
            }
        }
    }

    std::size_t getCollisionsNumber() const noexcept {
        return collisions_;
    }
};

#endif
