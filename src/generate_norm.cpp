#include "generate_norm.hpp"

#include <omp.h>
#include <cmath>
#include <iostream>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"
#include "gen.hpp"

Status generate_norm(size_t n, uint32_t seed, float mean, float stddev, float* result) {


const int N = static_cast<int>(n);

#pragma omp parallel 
    {

        int tid = omp_get_thread_num();
        int num_of_threads = omp_get_num_threads();

        int BLOCK_SIZE = divup(n, num_of_threads);

        int START = BLOCK_SIZE * tid;
        int END = std::min(BLOCK_SIZE * (tid + 1), N);
        Generator gen(seed, START);

        uint64_t b = (seed * fastPow(a, START + 1)) % mod;

        for (int i = START; i < END; i += 2) {
            float x = funi_f_distab(b, -1.0f, 1.0f);
            b *= a;
            b %= mod;
            float y = funi_f_distab(b, -1.0f, 1.0f);
            b *= a;
            b %= mod;
            float s = x * x + y * y;
            while (s == 0.0f || s > 1.0f) {
                x = funi_f_distab(b, -1.0f, 1.0f);
                b *= a;
                b %= mod;
                y = funi_f_distab(b, -1.0f, 1.0f);
                b *= a;
                b %= mod;
                s = x * x + y * y;
            }
            float z = sqrt(-2.0f * log(s) / s);
            float z0 = x * z;
            float z1 = y * z;
            result[i] = z0 * stddev + mean;
            result[i + 1] = z1 * stddev + mean;
        }
    }

    return STATUS_OK;
}