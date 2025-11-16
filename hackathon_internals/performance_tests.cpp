#include <cstdio>
#include <cstdint>
#include <chrono>
#include <string>
#include <iostream>
#include "tests.hpp"

struct BenchmarkResult {
    const char* function_name;
    int iterations;
    double time_ms;
};

void print_performance_table(BenchmarkResult* results, int count) {
    printf("┌─────────────────────┬────────────┬──────────────┐\n");
    printf("│ Function            │ Iterations │ Time (s)     │\n");
    printf("├─────────────────────┼────────────┼──────────────┤\n");
    
    for (int i = 0; i < count; i++) {
        printf("│ %-19s │ %10d │ %12.2f │\n",
               results[i].function_name,
               results[i].iterations,
               results[i].time_ms);
    }
    
    printf("└─────────────────────┴────────────┴──────────────┘\n");
}

template<typename Func, typename... Args>
double measure_time(Func func, const std::string& func_name, Args&&... args) {
    std::chrono::high_resolution_clock::time_point start_time, end_time;
    std::chrono::duration<double> elapsed;
    double best_time = std::numeric_limits<double>::max();
    double total_time = 0.0;
    std::size_t n_run = 0;

    Status s = func(std::forward<Args>(args)...);

    while ((total_time < 3) || (n_run < 3)) {
        start_time = std::chrono::high_resolution_clock::now();
        s = func(std::forward<Args>(args)...);
        end_time = std::chrono::high_resolution_clock::now();
        elapsed = end_time - start_time;
        best_time = std::min(best_time, elapsed.count());
        total_time += elapsed.count();
        ++n_run;
    }
    if (s) {
        std::cerr << "Return status of '" << func_name << " function' not ok." << std::endl;
    }

    return best_time;
}

BenchmarkResult benchmark_bits() {
    int N = 1000000000;
    std::vector<uint32_t> result(N);
   
    Status (* volatile generate_bits_ptr)(size_t, uint32_t, uint32_t*) = &generate_bits;
    double best_time = measure_time(generate_bits_ptr, "bits", N, 42, result.data());

    return {"bits", N, best_time};
}

BenchmarkResult benchmark_uniform() {
    int N = 1000000000;
    std::vector<float> result(N);

    Status (* volatile generate_uniform_ptr)(size_t, uint32_t, float, float, float*) = &generate_uniform;
    double best_time = measure_time(generate_uniform_ptr, "uniform", N, 42, 1.0f, 10.0f, result.data());

    return {"uniform", N, best_time};
}

BenchmarkResult benchmark_norm() {
    int N = 500000000;
    std::vector<float> result(N);

    Status (* volatile generate_norm_ptr)(size_t, uint32_t, float, float, float*) = &generate_norm;
    double best_time = measure_time(generate_norm_ptr, "norm", N, 42, 0.0f, 1.0f, result.data());

    return {"norm", N, best_time};
}

BenchmarkResult benchmark_exponential() {
    int N = 1000000000;
    std::vector<float> result(N);

    Status (* volatile generate_exponential_ptr)(size_t, uint32_t, float, float*) = &generate_exponential;
    double best_time = measure_time(generate_exponential_ptr, "exponential", N, 42, 2.0f, result.data());

    return {"exponential", N, best_time};
}

BenchmarkResult benchmark_bernoulli() {
    int N = 500000000;
    std::vector<float> result(N);

    Status (* volatile generate_bernoulli_ptr)(size_t, uint32_t, float, float*) = &generate_bernoulli;
    double best_time = measure_time(generate_bernoulli_ptr, "bernoulli", N, 42, 0.5f, result.data());

    return {"bernoulli", N, best_time};
}

BenchmarkResult benchmark_aes256_gcm() {
    int N = 2097152;
    std::vector<uint8_t> plaintext(N, 7);
    std::vector<uint8_t> ciphertext(N, 0);
    std::vector<uint8_t> key(32, 1);
    std::vector<uint8_t> iv(12, 2);
    uint8_t tag[16];

    Status (* volatile aes256_gcm_ptr)(const uint8_t*, uint8_t*, const uint8_t*, const uint8_t*, size_t, uint8_t*) = &aes256_gcm;
    double best_time = measure_time(aes256_gcm_ptr, "aes256_gcm", plaintext.data(), ciphertext.data(), key.data(), iv.data(), N, tag);

    return {"aes256_gcm", N, best_time};
}

BenchmarkResult benchmark_crc32() {
    int N = 500000000;
    std::vector<uint8_t> data(N);
    uint32_t result;

    Status (* volatile crc32_ptr)(const uint8_t*, size_t, uint32_t*) = &crc32;
    double best_time = measure_time(crc32_ptr, "crc32", data.data(), N, &result);

    return {"crc32", N, best_time};
}


int run_performance() {
    BenchmarkResult results[7];
    results[0] = benchmark_bits();
    results[1] = benchmark_uniform();
    results[2] = benchmark_norm();
    results[3] = benchmark_exponential();
    results[4] = benchmark_bernoulli();
    results[5] = benchmark_aes256_gcm();
    results[6] = benchmark_crc32();
    
    print_performance_table(results, 7);
    
    return 0;
}
