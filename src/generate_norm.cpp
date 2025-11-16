#include "generate_norm.hpp"

#include <omp.h>
#include <cmath>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"

Status generate_norm(size_t n, uint32_t seed, float mean, float stddev, float* result) {
    std::minstd_rand0 gen(seed);
    std::normal_distribution<float> d{ mean, stddev };

    for (int i = 0; i < n; ++i) {
        result[i] = d(gen);
    }

    return STATUS_OK;
}