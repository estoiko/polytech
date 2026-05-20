#include <cassert>
#include <cstddef>
#include <string>
#include <sstream>
#include <utility>
#include "String.h"
#include <iostream>
#include "Strutils.h"
#include <random>

String make_big_string(std::size_t n) {
    String result;
    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist('A', 'Z');

    for (std::size_t i = 0; i < n; ++i) {
        char c = static_cast<char>(dist(rng));
        char buf[2] = { c, '\0' };
        result += buf; 
    }

    return result;
}

int main() {

    const std::size_t N = 100'000'000;      
    const int REPEATS = 10;                 

    String big = make_big_string(N);

    volatile std::size_t checksum = 0;      

    for (int i = 0; i < REPEATS; ++i) {
        String lower_str = big.lower();
        checksum += lower_str.length();
    }

    // std::cout << "Checksum: " << checksum << '\n';
    // std::cout << "All done.\n";
    return 0;
}