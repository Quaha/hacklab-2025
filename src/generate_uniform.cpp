#include "generate_uniform.hpp"

#include <omp.h>
#include <cmath>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"

Status generate_uniform(size_t n, uint32_t seed, float min, float max, float* result) {
    std::minstd_rand0 gen(seed);
    std::uniform_real_distribution<float> d{ min, max };

    for (int i = 0; i < n; ++i) {
        result[i] = d(gen);
    }
    return STATUS_OK;
}