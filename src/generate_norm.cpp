#include "generate_norm.hpp"

#include <omp.h>
#include <cmath>
#include <cstddef> 
#include <cstdint>
#include <random>

#include "utils.hpp"
#include "gen.hpp"

struct Zic{
    float stairWidth[257], stairHeight[256];
    const float x1 = 3.6541528853610088f;
    const float A = 4.92867323399e-3f; /// area under rectangle

    Zic() {
        // coordinates of the implicit rectangle in base layer
        stairHeight[0] = exp(-0.5f * x1 * x1);
        stairWidth[0] = A / stairHeight[0];
        // implicit value for the top layer
        stairWidth[256] = 0.0f;
        for (unsigned int i = 1; i <= 255; ++i)
        {
            // such x_i that f(x_i) = y_{i-1}
            stairWidth[i] = sqrtf(-2.0f * log(stairHeight[i - 1]));
            stairHeight[i] = stairHeight[i - 1] + A / stairWidth[i];
        }
    }

    float NormalZiggurat(Generator &gen) {
        int iter = 0;
        do {
            uint64_t B = gen();
            int stairId = B & 255;
            float x = uni_f_distab(gen,0, stairWidth[stairId]); // get horizontal coordinate
            if (x < stairWidth[stairId + 1])
                return ((signed)B > 1073741823) ? x : -x;
            if (stairId == 0) // handle the base layer
            {
                static float z = -1.0f;
                float y;
                if (z > 0.0f) // we don't have to generate another exponential variable as we already have one
                {
                    x = exp_f_dist(gen,x1);
                    z -= 0.5f * x * x;
                }
                if (z <= 0.0f) // if previous generation wasn't successful
                {
                    do {
                        x = exp_f_dist(gen,x1);
                        y = exp_f_dist(gen,1);
                        z = y - 0.5f * x * x; // we storage this value as after acceptance it becomes exponentially distributed
                    } while (z <= 0.0f);
                }
                x += x1;
                return ((signed)B > 1073741823) ? x : -x;
            }
            // handle the wedges of other stairs
            if (uni_f_distab(gen,stairHeight[stairId - 1], stairHeight[stairId]) < exp(-0.5f * x * x))
                return ((signed)B > 1073741823) ? x : -x;
        } while (++iter <= 1e9); /// one billion should be enough
        return NAN; /// fail due to some error
    }

    float Normal(float mu, float sigma, Generator& gen) {
        return mu + NormalZiggurat(gen) * sigma;
    }
};
Status generate_norm(size_t n, uint32_t seed, float mean, float stddev, float* result) {


    const int N = static_cast<int>(n);
    Zic d;
#pragma omp parallel 
    {

        int tid = omp_get_thread_num();
        int num_of_threads = omp_get_num_threads();

        int BLOCK_SIZE = divup(n, num_of_threads);

        int START = BLOCK_SIZE * tid;
        int END = std::min(BLOCK_SIZE * (tid + 1), N);
        Generator gen(seed, START);

        
        //std::normal_distribution<float> d2{ mean, stddev };

        for (int i = START; i < END; ++i) {
           // result[i] = d2(gen);
            result[i] = d.Normal(mean, stddev,gen);
           // std::cout << result[i] << "!\n";
            
        }
    }

    return STATUS_OK;
}