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

        uint64_t b = (seed * fastPow(a, START + 1)) % mod;

        for (int i = START; i < END; ++i) {
            result[i] = funi_f_distab(b, min, max);
            b *= a;
            b %= mod;
        }
    }

    return STATUS_OK;
}