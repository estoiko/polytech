#include "Strutils.h"

#include <cstddef>
#include <iostream>

namespace my {
    size_t strlen(const char* str) {
        size_t length = 0;
        while (*str != '\0') {
            ++length;
            ++str;
        }
        return length;
    }

    char* strcpy(char* dest, const char* src) {
        char* cpydest = dest;
        while (*src) {
            *dest = *src;
            ++dest;
            ++src;
        }
        *dest = '\0';
        return cpydest;
    }

}

