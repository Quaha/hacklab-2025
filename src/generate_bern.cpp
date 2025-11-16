#include "generate_bern.hpp"

#include <omp.h>
#include <cmath>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"
#include "gen.hpp"

Status generate_bernoulli(size_t n, uint32_t seed, float probability, float* result) {

    const int N = static_cast<int>(n);

#pragma omp parallel 
    {

        int tid = omp_get_thread_num();
        int num_of_threads = omp_get_num_threads();

        int BLOCK_SIZE = divup(n, num_of_threads);

        int START = BLOCK_SIZE * tid;
        int END = std::min(BLOCK_SIZE * (tid + 1), N);

        Generator gen(seed, START);
        std::bernoulli_distribution d{ probability };

        for (int i = START; i < END; ++i) {
            result[i] = d(gen);
        }
    }

    return STATUS_OK;
}