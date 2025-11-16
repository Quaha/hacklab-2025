#pragma once

#include <cstdint>

constexpr uint64_t a = 16807ull;
constexpr uint64_t mod = 2147483647ull;


inline int divup(int x, int y) {
    return (x + y - 1) / y;
}

inline uint64_t fastPow(uint64_t v, uint64_t deg) {
    uint64_t ans = 1ull;
    while (deg) {
        if (deg&1ull) {
            ans *= v;
            ans %= mod;
        }
        v *= v;
        v %= mod;
        deg >>= 1ull;
    }
    return ans;
}