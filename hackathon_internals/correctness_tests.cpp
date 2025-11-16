#include <cstdio>
#include <cstdint>
#include <cstring>
#include <random>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "tests.hpp"

const TestCaseBits testcases_bits[NUM_OF_TESTCASES_BITS] = {
    {.n = 10000, .seed = 42},
    {.n = 100000, .seed = 84},
    {.n = 1000000, .seed = 168},
    {.n = 1000000, .seed = 336},
    {.n = 10000, .seed = 672},
    {.n = 10000, .seed = 1344},
    {.n = 100000, .seed = 2688},
    {.n = 10000, .seed = 5376},
    {.n = 100000, .seed = 10752},
    {.n = 1000000, .seed = 21504},
};

const TestCaseUniform testcases_uniform[NUM_OF_TESTCASES_UNIFORM] = {
    {.n = 10000, .seed = 42, .min = 0.0f, .max = 1.0f},
    {.n = 10000, .seed = 123, .min = 1.0f, .max = 1.5f},
    {.n = 100000, .seed = 777, .min = -2.0f, .max = 1.0f},
    {.n = 1000000, .seed = 11, .min = 3.0f, .max = 10.0f},
    {.n = 1000, .seed = 84, .min = -1.0f, .max = 2.0f},
    {.n = 10000, .seed = 168, .min = 0.5f, .max = 2.5f},
    {.n = 100000, .seed = 336, .min = 0.0f, .max = 3.0f},
    {.n = 1000000, .seed = 672, .min = 2.0f, .max = 5.0f},
    {.n = 100, .seed = 1344, .min = -3.0f, .max = 0.0f},
    {.n = 10000, .seed = 2688, .min = -1.0f, .max = 0.5f},
};

const TestCaseNorm testcases_norm[NUM_OF_TESTCASES_NORM] = {
    {.n = 1000, .seed = 35, .mean = 0.0f, .stddev = 1.0f},
    {.n = 10000, .seed = 123, .mean = 1.0f, .stddev = 0.5f},
    {.n = 100000, .seed = 777, .mean = 0.2f, .stddev = 10.0f},
    {.n = 1000000, .seed = 11, .mean = 10.0f, .stddev = 0.3f},
    {.n = 1000, .seed = 84, .mean = -1.0f, .stddev = 2.0f},
    {.n = 10000, .seed = 168, .mean = 0.5f, .stddev = 1.5f},
    {.n = 100000, .seed = 336, .mean = 1.2f, .stddev = 5.0f},
    {.n = 1000000, .seed = 672, .mean = 2.0f, .stddev = 0.5f},
    {.n = 1000, .seed = 1344, .mean = -2.0f, .stddev = 2.5f},
    {.n = 10000, .seed = 2688, .mean = 0.7f, .stddev = 3.0f},
};

const TestCaseExponential testcases_exponential[NUM_OF_TESTCASES_EXPONENTIAL] = {
    {.n = 100, .seed = 42, .lambda = 1.0f},
    {.n = 10000, .seed = 123, .lambda = 0.1f},
    {.n = 100000, .seed = 777, .lambda = 10.0f},
    {.n = 1000000, .seed = 11, .lambda = 111.0f},
    {.n = 1000, .seed = 84, .lambda = 0.5f},
    {.n = 10000, .seed = 168, .lambda = 2.0f},
    {.n = 100000, .seed = 336, .lambda = 5.0f},
    {.n = 1000000, .seed = 672, .lambda = 33.0f},
    {.n = 1000, .seed = 1344, .lambda = 0.01f},
    {.n = 10000, .seed = 2688, .lambda = 0.5f},
};

const TestCaseBernoulli testcases_bernoulli[NUM_OF_TESTCASES_BERNOULLI] = {
    {.n = 1000, .seed = 42, .probability = 0.4f},
    {.n = 1000, .seed = 123, .probability = 0.9f},
    {.n = 10000, .seed = 7, .probability = 0.501f},
    {.n = 100000, .seed = 333, .probability = 0.11f},
    {.n = 1000, .seed = 84, .probability = 0.3f},
    {.n = 1000, .seed = 168, .probability = 0.8f},
    {.n = 10000, .seed = 336, .probability = 0.5f},
    {.n = 100000, .seed = 672, .probability = 0.2f},
    {.n = 1000, .seed = 1344, .probability = 0.7f},
    {.n = 1000, .seed = 2688, .probability = 0.6f},
};

TestCaseAES testcases_aes[NUM_OF_TESTCASES_AES] = {
    {.n = 16,
    .key = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C, 0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA, 0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81},
    .iv = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B},
    .plaintext = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A},
    .ciphertext = {0x40, 0x72, 0x02, 0x68, 0xAD, 0x2E, 0x95, 0x41, 0xA5, 0x07, 0x66, 0xEB, 0xF2, 0x04, 0x40, 0x71},
    .tag = {0x4D, 0xCB, 0x82, 0x40, 0x3E, 0xCA, 0x19, 0x22, 0xCA, 0x78, 0x2F, 0x7B, 0x9B, 0x36, 0x40, 0xFB}},

    {.n = 32,
    .key = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F},
    .iv = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01},
    .plaintext = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF},
    .ciphertext = {0x15, 0xC7, 0x9D, 0xCF, 0x00, 0xA1, 0x56, 0x69, 0x86, 0xB7, 0xFB, 0x82, 0x20, 0x7B, 0xD4, 0x08, 0x14, 0xBC, 0x59, 0x33, 0xED, 0x19, 0x99, 0x6B, 0x72, 0x23, 0x46, 0xBE, 0xAF, 0x9B, 0xBE, 0xB1},
    .tag = {0x0C, 0xD2, 0x43, 0x49, 0x81, 0x8A, 0x9A, 0x75, 0x69, 0x80, 0xF2, 0x17, 0x2D, 0x8A, 0xFD, 0x82}},

    {.n = 64,
    .key = {0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C, 0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08, 0xFE, 0xFF, 0xE9, 0x92, 0x86, 0x65, 0x73, 0x1C, 0x6D, 0x6A, 0x8F, 0x94, 0x67, 0x30, 0x83, 0x08},
    .iv = {0xCA, 0xFE, 0xBA, 0xBE, 0xFA, 0xCE, 0xDB, 0xAD, 0xDE, 0xCA, 0xF8, 0x88},
    .plaintext = {0xD9, 0x31, 0x32, 0x25, 0xF8, 0x84, 0x06, 0xE5, 0xA5, 0x59, 0x09, 0xC5, 0xAF, 0xF5, 0x26, 0x9A, 0x86, 0xA7, 0xA9, 0x53, 0x15, 0x34, 0xF7, 0xDA, 0x2E, 0x4C, 0x30, 0x3D, 0x8A, 0x31, 0x8A, 0x72, 0x1C, 0x3C, 0x0C, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2F, 0xCF, 0x0E, 0x24, 0x49, 0xA6, 0xB5, 0x25, 0xB1, 0x6A, 0xED, 0xF5, 0xAA, 0x0D, 0xE6, 0x57, 0xBA, 0x63, 0x7B, 0x39, 0x1A, 0xAF, 0xD2, 0x55},
    .ciphertext = {0x52, 0x2D, 0xC1, 0xF0, 0x99, 0x56, 0x7D, 0x07, 0xF4, 0x7F, 0x37, 0xA3, 0x2A, 0x84, 0x42, 0x7D, 0x64, 0x3A, 0x8C, 0xDC, 0xBF, 0xE5, 0xC0, 0xC9, 0x75, 0x98, 0xA2, 0xBD, 0x25, 0x55, 0xD1, 0xAA, 0x8C, 0xB0, 0x8E, 0x48, 0x59, 0x0D, 0xBB, 0x3D, 0xA7, 0xB0, 0x8B, 0x10, 0x56, 0x82, 0x88, 0x38, 0xC5, 0xF6, 0x1E, 0x63, 0x93, 0xBA, 0x7A, 0x0A, 0xBC, 0xC9, 0xF6, 0x62, 0x89, 0x80, 0x15, 0xAD},
    .tag = {0xB0, 0x94, 0xDA, 0xC5, 0xD9, 0x34, 0x71, 0xBD, 0xEC, 0x1A, 0x50, 0x22, 0x70, 0xE3, 0xCC, 0x6C}},

    {.n = 128,
    .key = {0x1F, 0x8E, 0x0E, 0x3C, 0x1B, 0x3B, 0x1A, 0x4E, 0x2F, 0x7D, 0x4A, 0x5C, 0x6E, 0x8B, 0x9D, 0x2A, 0x3C, 0x5E, 0x7F, 0x90, 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6, 0x07, 0x18, 0x29, 0x3A, 0x4B, 0x5C},
    .iv = {0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98},
    .plaintext = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    .ciphertext = {0xDE, 0x3D, 0xED, 0xEF, 0x59, 0x7D, 0x95, 0xE2, 0x93, 0x2E, 0xB3, 0xE0, 0x3F, 0x2E, 0x26, 0xE5, 0x0B, 0x2D, 0x0E, 0x85, 0xB5, 0x90, 0x74, 0x7E, 0xE2, 0xF1, 0x11, 0x86, 0x03, 0x0B, 0xEA, 0xA1, 0xC9, 0x95, 0xE7, 0xC5, 0xD8, 0x2D, 0x88, 0xD0, 0x69, 0x29, 0xF9, 0x8F, 0x6F, 0xD7, 0xFD, 0xFE, 0x90, 0x84, 0xFF, 0x68, 0x09, 0xB3, 0x6F, 0x0A, 0x21, 0x6F, 0x43, 0x9E, 0x9B, 0x9E, 0x5A, 0x4A, 0xBE, 0xA7, 0xDB, 0xB3, 0x55, 0xCB, 0x63, 0x56, 0x36, 0xDA, 0x6C, 0x7F, 0xB8, 0xDE, 0x4A, 0x7F, 0x60, 0x03, 0xEE, 0xF6, 0xA3, 0x4B, 0xC3, 0xFC, 0xAE, 0x96, 0x63, 0x1C, 0xE5, 0x49, 0xA0, 0x6E, 0x91, 0x68, 0x26, 0x4D, 0x23, 0xCC, 0x63, 0xC0, 0xDB, 0x51, 0x1B, 0x22, 0x60, 0xAC, 0xCC, 0x52, 0x3E, 0x8A, 0xC7, 0xEB, 0x01, 0x3F, 0xAD, 0x8B, 0xF8, 0x2F, 0x97, 0x96, 0xD3, 0x0F, 0x1D, 0xE3},
    .tag = {0xD8, 0x34, 0x7D, 0xEB, 0x0E, 0x6D, 0x2C, 0xB9, 0x87, 0x13, 0xE2, 0x5E, 0x7B, 0x52, 0x16, 0xC5}},
};

TestCaseCRC32 testcases_crc32[NUM_OF_TESTCASES_CRC32] = {
    {
        .input = "Hello, World!",
        .result = 0xEC4AC3D0,
    },
    {
        .input = "abcdefghijklmnopqrstuvwxyz",
        .result = 0x4C2750BD,
    },
    {
        .input = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        .result = 0x1FC2E6D2,
    },
    {
        .input = "a",
        .result = 0xE8B7BE43,
    },
    {
        .input = "привет",
        .result = 0x2E763E4E,
    },
    {
        .input = "!@#$%^&*()",
        .result = 0xAEA29B98,
    },
    {
        .input = "",
        .result = 0x00000000,
    },
    {
        .input = "The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog.",
        .result = 0x9AE90AA7,
    },
    {
        .input = "1234567890",
        .result = 0x261DAEE5,
    },
    {
        .input = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
        .result = 0x414C623C,
    },
};

SampleMoments get_sample_moments(uint32_t n, float *result) {
    SampleMoments sample;
    sample.m1 = 0.0f;
    sample.m2 = 0.0f;

    for (uint32_t i = 0; i < n; ++i) { 
        sample.m1 += result[i];
    }
    sample.m1 /= static_cast<float>(n);

    for (uint32_t i = 0; i < n; ++i) {
        sample.m2 += (result[i] - sample.m1) * (result[i] - sample.m1);
    }
    sample.m2 /= static_cast<float>(n);

    return sample;
}

bool test_bits() {
    printf("Running bits generation tests...\n");
    
    for (int i = 0; i < NUM_OF_TESTCASES_BITS; ++i) {
        printf("  Test %d: n = %zu, seed = %u\n", i + 1, testcases_bits[i].n, testcases_bits[i].seed);
        
        std::vector<uint32_t> reference(testcases_bits[i].n);
        std::vector<uint32_t> result(testcases_bits[i].n);
        
        Status s = generate_bits(testcases_bits[i].n, testcases_bits[i].seed, result.data());
        if (s) {
            printf("    ERROR: failed calling generate_bits, status = %d\n", s);
            return false; 
        }
        
        std::minstd_rand0 gen(testcases_bits[i].seed);
        for (uint32_t j = 0; j < testcases_bits[i].n; ++j) {
            reference[j] = gen();
            if (reference[j] != result[j]) {
                printf("    ERROR: bit-to-bit comparison failed at position %u\n", j);
                printf("    Expected: 0x%08X, Got: 0x%08X\n", reference[j], result[j]);
                return false;
            }
        }
        
        printf("    OK\n");
    }
    
    printf("test_bits: OK\n");
    return true;
}

bool test_uniform() {
    printf("Running uniform distribution tests...\n");
    const float alpha = 5.0f; 
    
    for (int i = 0; i < NUM_OF_TESTCASES_UNIFORM; ++i) {
        printf("  Test %d: n = %zu, seed = %u, range [%.2f, %.2f]\n", 
               i + 1, testcases_uniform[i].n, testcases_uniform[i].seed, 
               testcases_uniform[i].min, testcases_uniform[i].max);
        
        std::vector<float> result(testcases_uniform[i].n);
        Status s = generate_uniform(testcases_uniform[i].n, testcases_uniform[i].seed, testcases_uniform[i].min, testcases_uniform[i].max, result.data());
        if (s) {
            printf("    ERROR: failed calling generate_uniform, status = %d\n", s);
            return false; 
        }
        
        for (uint32_t j = 0; j < testcases_uniform[i].n; ++j) {
            if ((!std::isfinite(result[j])) | (result[j] < testcases_uniform[i].min) | (result[j] > testcases_uniform[i].max)) {
                printf("    ERROR: value at position %u is Inf/NaN or outside interval\n", j);
                return false;
            }
        }
        
        SampleMoments sample = get_sample_moments(testcases_uniform[i].n, result.data());
        float theoretical_m1 = (testcases_uniform[i].max + testcases_uniform[i].min) / 2.0f;
        float theoretical_m2 = (testcases_uniform[i].max - testcases_uniform[i].min) * (testcases_uniform[i].max - testcases_uniform[i].min) / 12.0f;
        float theoretical_m4 = std::pow(testcases_uniform[i].max - testcases_uniform[i].min, 4.0f) / 80.0f;
        float eps1 = alpha * std::sqrt(theoretical_m2 / testcases_uniform[i].n); 
        float eps2 = alpha * std::sqrt(theoretical_m4 / testcases_uniform[i].n); 
        bool m1_ok = std::fabs(theoretical_m1 - sample.m1) < eps1;
        bool m2_ok = std::fabs(theoretical_m2 - sample.m2) < eps2;
        if (!(m1_ok & m2_ok)) { 
            printf("    ERROR: statistical moments don't match expectations\n");
            printf("    Expected: 1st = %.6f, 2nd = %.6f\n", theoretical_m1, theoretical_m2);
            printf("    Got:      1st = %.6f, 2nd = %.6f\n", sample.m1, sample.m2);
            printf("    Allowed:  1st = %.6f, 2nd = %.6f\n", eps1, eps2);
            return false; 
        }
        
        printf("    OK (1st moment: %.6f, 2nd moment: %.6f)\n", sample.m1, sample.m2);
    }
    
    printf("test_uniform: OK\n");
    return true;
}

bool test_norm() {
    printf("Running normal distribution tests...\n");
    const float alpha = 5.0f; 
    
    for (int i = 0; i < NUM_OF_TESTCASES_NORM; ++i) {
        printf("  Test %d: n = %zu, seed = %u, mean = %.2f, stddev = %.2f\n", 
               i + 1, testcases_norm[i].n, testcases_norm[i].seed, 
               testcases_norm[i].mean, testcases_norm[i].stddev);
        
        std::vector<float> result(testcases_norm[i].n);
        Status s = generate_norm(testcases_norm[i].n, testcases_norm[i].seed, testcases_norm[i].mean, testcases_norm[i].stddev, result.data());
        if (s) {
            printf("    ERROR: failed calling generate_norm, status = %d\n", s);
            return false; 
        }
        
        for (uint32_t j = 0; j < testcases_norm[i].n; ++j) {
            if (!std::isfinite(result[j])) {
                printf("    ERROR: value at position %u is Inf or NaN\n", j);
                return false;
            }
        }
        
        SampleMoments sample = get_sample_moments(testcases_norm[i].n, result.data());
        float theoretical_m1 = testcases_norm[i].mean;
        float theoretical_m2 = testcases_norm[i].stddev * testcases_norm[i].stddev;
        float theoretical_m4 = std::pow(testcases_norm[i].stddev, 4.0f) * 3.0f;
        float eps1 = alpha * std::sqrt(theoretical_m2 / testcases_norm[i].n); 
        float eps2 = alpha * std::sqrt(theoretical_m4 / testcases_norm[i].n); 
        bool m1_ok = std::fabs(theoretical_m1 - sample.m1) < eps1;
        bool m2_ok = std::fabs(theoretical_m2 - sample.m2) < eps2;
        if (!(m1_ok & m2_ok)) { 
            printf("    ERROR: statistical moments don't match expectations\n");
            printf("    Expected: 1st = %.6f, 2nd = %.6f\n", theoretical_m1, theoretical_m2);
            printf("    Got:      1st = %.6f, 2nd = %.6f\n", sample.m1, sample.m2);
            printf("    Allowed:  1st = %.6f, 2nd = %.6f\n", eps1, eps2);
            return false; 
        }
        
        printf("    OK (1st moment: %.6f, 2nd moment: %.6f)\n", sample.m1, sample.m2);
    }
    
    printf("test_norm: OK\n");
    return true;
}

bool test_exponential() {
    printf("Running exponential distribution tests...\n");
    const float alpha = 5.0f; 
    
    for (int i = 0; i < NUM_OF_TESTCASES_EXPONENTIAL; ++i) {
        printf("  Test %d: n = %zu, seed = %u, lambda = %.2f\n", 
               i + 1, testcases_exponential[i].n, testcases_exponential[i].seed, 
               testcases_exponential[i].lambda);
        
        std::vector<float> result(testcases_exponential[i].n);
        Status s = generate_exponential(testcases_exponential[i].n, testcases_exponential[i].seed, testcases_exponential[i].lambda, result.data());
        if (s) {
            printf("    ERROR: failed calling generate_exponential, status = %d\n", s);
            return false; 
        }
        
        for (uint32_t j = 0; j < testcases_exponential[i].n; ++j) {
            if ((!std::isfinite(result[j])) | (result[j] < 0.0f)) {
                printf("    ERROR: value at position %u is Inf/NaN or negative\n", j);
                return false;
            }
        }
        
        SampleMoments sample = get_sample_moments(testcases_exponential[i].n, result.data());
        float theoretical_m1 = 1.0f / testcases_exponential[i].lambda;
        float theoretical_m2 = 1.0f / std::pow(testcases_exponential[i].lambda, 2.0f);
        float theoretical_m4 = 9.0f / std::pow(testcases_exponential[i].lambda, 4.0f);
        float eps1 = alpha * std::sqrt(theoretical_m2 / testcases_exponential[i].n); 
        float eps2 = alpha * std::sqrt(theoretical_m4 / testcases_exponential[i].n); 
        bool m1_ok = std::fabs(theoretical_m1 - sample.m1) < eps1;
        bool m2_ok = std::fabs(theoretical_m2 - sample.m2) < eps2;
        if (!(m1_ok & m2_ok)) { 
            printf("    ERROR: statistical moments don't match expectations\n");
            printf("    Expected: 1st = %.6f, 2nd = %.6f\n", theoretical_m1, theoretical_m2);
            printf("    Got:      1st = %.6f, 2nd = %.6f\n", sample.m1, sample.m2);
            printf("    Allowed:  1st = %.6f, 2nd = %.6f\n", eps1, eps2);
            return false; 
        }
        
        printf("    OK (1st moment: %.6f, 2nd moment: %.6f)\n", sample.m1, sample.m2);
    }
    
    printf("test_exponential: OK\n");
    return true;
}

bool test_bernoulli() {
    printf("Running Bernoulli distribution tests...\n");
    const float alpha = 5.0f; 
    
    for (int i = 0; i < NUM_OF_TESTCASES_BERNOULLI; ++i) {
        printf("  Test %d: n = %zu, seed = %u, probability = %.3f\n", 
               i + 1, testcases_bernoulli[i].n, testcases_bernoulli[i].seed, 
               testcases_bernoulli[i].probability);
        
        std::vector<float> result(testcases_bernoulli[i].n);
        Status s = generate_bernoulli(testcases_bernoulli[i].n, testcases_bernoulli[i].seed, testcases_bernoulli[i].probability, result.data());
        if (s) {
            printf("    ERROR: failed calling generate_bernoulli, status = %d\n", s);
            return false;
        }
        
        for (uint32_t j = 0; j < testcases_bernoulli[i].n; ++j) {
            if ((!std::isfinite(result[j])) | ((result[j] != 0.0f) & (result[j] != 1.0f))) {
                printf("    ERROR: value at position %u is Inf/NaN or not 0/1\n", j);
                return false;
            }
        }
        
        SampleMoments sample = get_sample_moments(testcases_bernoulli[i].n, result.data());
        float theoretical_m1 = testcases_bernoulli[i].probability;
        float theoretical_m2 = testcases_bernoulli[i].probability * (1.0f - testcases_bernoulli[i].probability);
        float theoretical_m4 = -3.0f * std::pow(testcases_bernoulli[i].probability, 4.0f) + 6.0f * std::pow(testcases_bernoulli[i].probability, 3.0f) - 4.0f * std::pow(testcases_bernoulli[i].probability, 2.0f) + testcases_bernoulli[i].probability;
        float eps1 = alpha * std::sqrt(theoretical_m2 / testcases_bernoulli[i].n); 
        float eps2 = alpha * std::sqrt(theoretical_m4 / testcases_bernoulli[i].n); 
        bool m1_ok = std::fabs(theoretical_m1 - sample.m1) < eps1;
        bool m2_ok = std::fabs(theoretical_m2 - sample.m2) < eps2;
        if (!(m1_ok & m2_ok)) { 
            printf("    ERROR: statistical moments don't match expectations\n");
            printf("    Expected: 1st = %.6f, 2nd = %.6f\n", theoretical_m1, theoretical_m2);
            printf("    Got:      1st = %.6f, 2nd = %.6f\n", sample.m1, sample.m2);
            printf("    Allowed:  1st = %.6f, 2nd = %.6f\n", eps1, eps2);
            return false; 
        }
        
        printf("    OK (1st moment: %.6f, 2nd moment: %.6f)\n", sample.m1, sample.m2);
    }
    
    printf("test_bernoulli: OK\n");
    return true;
}


bool test_aes256_gcm() {
    printf("Running AES-256-GCM tests...\n");
    
    for (size_t i = 0; i < NUM_OF_TESTCASES_AES; ++i) {
        printf("  Test %zu: data size = %zu bytes\n", i + 1, testcases_aes[i].n);
        
        std::vector<uint8_t> answer;
        answer.resize(testcases_aes[i].n);
        
        uint8_t tag[16];
        
        if (testcases_aes[i].key.size() != 32) {
            printf("    ERROR: invalid key size: %zu (expected 32)\n", testcases_aes[i].key.size());
            return false;
        }
        
        if (testcases_aes[i].iv.size() != 12) {
            printf("    ERROR: invalid IV size: %zu (expected 12)\n", testcases_aes[i].iv.size());
            return false;
        }
        
        Status status = aes256_gcm(testcases_aes[i].plaintext.data(), answer.data(),
                                   testcases_aes[i].key.data(), testcases_aes[i].iv.data(), 
                                   testcases_aes[i].n, tag);
        
        if (status != STATUS_OK) {
            printf("    ERROR: aes256_gcm function returned status %d\n", status);
            return false;
        }
        
        printf("    Ciphertext: ");
        for (size_t j = 0; j < testcases_aes[i].n; j++) {
            printf("%02X", answer[j]);
        }
        printf("\n");
        
        printf("    Tag: ");
        for (int j = 0; j < 16; j++) {
            printf("%02X", tag[j]);
        }
        printf("\n");
        
        bool ciphertext_match = true;
        size_t first_ciphertext_error = 0;
        for (size_t j = 0; j < testcases_aes[i].n; j++) {
            if (answer[j] != testcases_aes[i].ciphertext[j]) {
                ciphertext_match = false;
                first_ciphertext_error = j;
                break;
            }
        }
        
        bool tag_match = true;
        size_t first_tag_error = 0;
        for (int j = 0; j < 16; j++) {
            if (tag[j] != testcases_aes[i].tag[j]) {
                tag_match = false;
                first_tag_error = j;
                break;
            }
        }
        
        if (!ciphertext_match) {
            printf("    ERROR: Ciphertext mismatch at byte %zu\n", first_ciphertext_error);
            printf("    Expected: 0x%02X, Got: 0x%02X\n", 
                   testcases_aes[i].ciphertext[first_ciphertext_error], answer[first_ciphertext_error]);
            return false;
        }
        
        if (!tag_match) {
            printf("    ERROR: Tag mismatch at byte %zu\n", first_tag_error);
            printf("    Expected: 0x%02X, Got: 0x%02X\n", 
                   testcases_aes[i].tag[first_tag_error], tag[first_tag_error]);
            return false;
        }
        
        printf("    OK (Matches OpenSSL reference)\n");
    }
    
    printf("test_aes256_gcm: OK\n");
    return true;
}

bool test_crc32() {
    printf("Running CRC32 tests...\n");
    
    for (size_t i = 0; i < NUM_OF_TESTCASES_CRC32; ++i) {
        printf("  Test %zu: \"%s\" (%zu bytes)\n", i + 1, 
               testcases_crc32[i].input.empty() ? "(empty string)" : testcases_crc32[i].input.c_str(),
               testcases_crc32[i].input.size());
        
        uint32_t result;
        Status status = crc32((const uint8_t*)(testcases_crc32[i].input.data()), 
                             testcases_crc32[i].input.size(), &result);
        
        if (status != STATUS_OK) {
            printf("    ERROR: crc32 function returned status %d\n", status);
            return false;
        }
        
        if (result != testcases_crc32[i].result) {
            printf("    ERROR: CRC32 mismatch\n");
            printf("    Expected: 0x%08X, Got: 0x%08X\n", 
                   testcases_crc32[i].result, result);
            return false;
        }
        
        printf("    OK (CRC32: 0x%08X)\n", result);
    }
    
    printf("test_crc32: OK\n");
    return true;
}


int run_correctness() {
    bool all_tests_passed = true;

    all_tests_passed &= test_bits();
    all_tests_passed &= test_uniform();
    all_tests_passed &= test_norm();
    all_tests_passed &= test_exponential();
    all_tests_passed &= test_bernoulli();
    all_tests_passed &= test_aes256_gcm();
    all_tests_passed &= test_crc32();
    
    if (!all_tests_passed) {
        printf("Correctness tests failed\n");
        return 1;
    }
    
    printf("All correctness tests passed\n");
    
    return 0;
}
