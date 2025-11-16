#pragma once

#include <cstdint>

inline int divup(int x, int y) {
    return (x + y - 1) / y;
}

inline uint32_t changeSeed(uint32_t seed, int tid) {

    uint32_t result = 0;

    uint32_t k = 7;
    uint32_t b = 1000000007;

    for (int i = 0; i < tid; ++i) {
        seed *= k;
        seed += b;
        result += seed;
    }

    return result;
}