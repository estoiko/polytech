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

bool String::operator==(const String& other) const {
    if (this == &other) {
        return true;
    }
    if (length_ != other.length_) {
        return false;
    }
    for (size_t i = 0; i < length_; ++i) {
        if (data_[i] != other.data_[i]) {
            return false;
        }
    }
    return true;
}

bool String::operator<(const String& other) const {
    size_t minLen = std::min(length_, other.length_);
    for (size_t i = 0; i < minLen; ++i) {
        if (data_[i] < other.data_[i]) {
            return true;
        }
        if (data_[i] > other.data_[i]) {
            return false;
        }
    }
    return length_ < other.length_;
}

bool String::operator>(const String& other) const {
    return other < *this;
}

bool String::operator<=(const String& other) const {
    return !(other < *this);
}

bool String::operator>=(const String& other) const {
    return !(*this < other);
}

bool String::operator!=(const String& other) const {
    return !(*this == other);
}

char& String::operator[](size_t index) {
    return data_[index];
}

const char& String::operator[](size_t index) const {
    return data_[index];
}

void String::clear() {
        delete[] data_;
        data_ = new char[1]{'\0'};
        length_ = 0;
        capacity_ = 1;
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
        capacity_ = newLen + 1;
        char* newData = new char[capacity_];

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

std::istream& operator>>(std::istream& stream, String& str) {
    str.clear();
    char ch = 0;

    while (stream.get(ch) && std::isspace(ch)) {
        
    }

    if (!stream) {
        return stream;
    }

    char buf[2] = { ch, '\0' };
    str += buf;
    
    while (stream.get(ch) && !std::isspace(ch)) {
        char buf[2] = { ch, '\0' };
        str += buf;
    }

    return stream;
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