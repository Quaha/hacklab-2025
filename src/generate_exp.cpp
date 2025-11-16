#include "generate_exp.hpp"

#include <omp.h>
#include <cmath>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"

Status generate_exponential(size_t n, uint32_t seed, float lambda, float* result) {

    std::minstd_rand0 gen(seed);
    std::exponential_distribution<float> d{ lambda };

    for (int i = 0; i < n; ++i) {
        result[i] = d(gen);
    }

    return STATUS_OK;
}