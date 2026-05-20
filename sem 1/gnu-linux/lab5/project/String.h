#ifndef STRING_H
#define STRING_H

#include <cstddef>
#include <iosfwd>
#include "Strutils.h"

class String {
public:  
    String():
        data_(new char[1]{'\0'}), 
        length_(0), 
        capacity_(1)
    {}

    String(const char* str): 
        length_(my::strlen(str)) 
    {
        capacity_ = length_ + 1;
        data_ = new char[capacity_];
        my::strcpy(data_, str);
    }

    ~String() {
        delete[] data_;
        data_ = nullptr;
    }

    String(const String& other)
        : data_(new char[other.capacity_]),
          length_(other.length_),
          capacity_(other.capacity_)
    {
        my::strcpy(data_, other.data_);
    }

    String(String&& other) noexcept
        : data_(other.data_), 
          length_(other.length_), 
          capacity_(other.capacity_) 
    {
        other.data_ = nullptr;
        other.length_ = 0;
        other.capacity_ = 0;
    }

    void swap(String& other) noexcept;

    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    String& operator=(const char* str);

    size_t length() const;
    bool empty() const;

    char& operator[](size_t index);
    const char& operator[](size_t index) const;

    String& operator+=(const String& other);
    String operator+(const String& other) const;
    String& operator+=(const char* str);

    friend std::ostream& operator<<(std::ostream& stream, const String& str);

    String lower() const;

private:  
    char* data_;
    size_t length_;
    size_t capacity_;
};

#endif