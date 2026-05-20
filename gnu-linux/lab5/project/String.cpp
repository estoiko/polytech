#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <cctype>
#include "String.h"

void String::swap(String& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(length_, other.length_);
    std::swap(capacity_, other.capacity_);
}

String& String::operator=(const String& other) {
    if (this != &other) {
        String temp(other);
        swap(temp);
    }
    return *this;
}

String& String::operator=(String&& other) noexcept {
    if (this != &other) {
        String temp(std::move(other));
        swap(temp);
    }
    return *this;
}

String& String::operator=(const char* str) {
    String temp(str);
    swap(temp);
    return *this;
}

size_t String::length() const {
    return length_;
}

bool String::empty() const {
    return length_ == 0;
}

char& String::operator[](size_t index) {
    if (index < 0 || index > length_ - 1) 
        throw std::out_of_range("ERROR: string index out of bounds"); 
    return data_[index];
}

const char& String::operator[](size_t index) const {
    if (index < 0 || index > length_ - 1) 
        throw std::out_of_range("ERROR: string index out of bounds"); 
    return data_[index];
}

String& String::operator+=(const String& other) {
    if (other.length_ == 0 || other.data_ == nullptr) {
        return *this;
    }

    if (this == &other) {
        String temp(other);  
        return *this += temp;
    }

    size_t newLen = length_ + other.length_;
    if (newLen + 1 <= capacity_) {
        my::strcpy(data_ + length_, other.data_);
    } else {
        capacity_ = (newLen + 1) * 2;
        char* newData = new char[capacity_ * 2] {};

        my::strcpy(newData, data_);
        my::strcpy(newData + length_, other.data_);

        delete[] data_;
        data_ = newData;
    }
    length_ = newLen;
    return *this;
}

String String::operator+(const String& other) const {
    String result(*this);   
    result += other;        
    return result;          
}

String& String::operator+=(const char* str) {
    return *this += String(str);
}

std::ostream& operator<<(std::ostream& stream, const String& str) {
    if (!str.data_) {
        return stream;
    }
    return stream << str.data_;
}

/* 11) Сформировать новую строку, заменив в исходной строке все прописные латинские буквы на
строчные. */

String String::lower() const {
    String newString{*this};
    int delta = 'a' - 'A';

    for (size_t i = 0; i < newString.length_; ++i) {
        if (newString[i] >= 'A' && newString[i] <= 'Z' && std::isupper(newString[i])) {
            newString[i] = newString[i] + delta;
        }
    }

    return newString;
}