#include "generate_bits.hpp"

#include <omp.h>
#include <cmath>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"
#include "gen.hpp"

#include <iostream>

Status generate_bits(size_t n, uint32_t seed, uint32_t* result) {

    const int N = static_cast<int>(n);

#pragma omp parallel 
    {

        int tid = omp_get_thread_num();
        int num_of_threads = omp_get_num_threads();

        int BLOCK_SIZE = divup(n, num_of_threads);

        int START = BLOCK_SIZE * tid;
        int END = std::min(BLOCK_SIZE * (tid + 1), N);

        VGenerator gen(seed, START);
        Generator sgen(seed, START);

        int i = START;
        for (; i + VGENSZ <= END; i += VGENSZ) {
            //for (int j = 0; j < VGENSZ; j++) {
            //    if (i != START) continue;
            //    std::cout << sgen() << " ";
            //}
            gen.copy(VGENSZ, i, result);
            //for (int j = 0; j < VGENSZ; j++) {
            //    if (i != START) continue;
            //    std::cout << gen.state[j] << " ";
            //}
            //std::cout << "\n";
            gen.change();
        }
        gen.copy(END - i, i, result);
    }

    return STATUS_OK;
}