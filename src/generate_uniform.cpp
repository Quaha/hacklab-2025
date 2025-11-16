#include "generate_uniform.hpp"

#include <omp.h>
#include <cmath>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"
#include "gen.hpp"

Status generate_uniform(size_t n, uint32_t seed, float min, float max, float* result) {

    const int N = static_cast<int>(n);

#pragma omp parallel 
    {

        int tid = omp_get_thread_num();
        int num_of_threads = omp_get_num_threads();

        int BLOCK_SIZE = divup(n, num_of_threads);

        int START = BLOCK_SIZE * tid;
        int END = std::min(BLOCK_SIZE * (tid + 1), N);

        Generator gen(seed, START);

        for (int i = START; i < END; ++i) {
            result[i] = uni_f_distab(gen, min, max);
        }
    }

    return STATUS_OK;
}