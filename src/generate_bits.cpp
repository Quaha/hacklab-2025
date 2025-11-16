#include "generate_bits.hpp"

#include <omp.h>
#include <cmath>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"

Status generate_bits(size_t n, uint32_t seed, uint32_t* result) {
    std::minstd_rand0 gen(seed);

    for (int i = 0; i < n; ++i) {
        result[i] = 0;
    }
    return STATUS_OK;
}