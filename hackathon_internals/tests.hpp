#include <vector>
#include <string>

#include "../src/solution.hpp"

#define NUM_OF_TESTCASES_BITS 10
#define NUM_OF_TESTCASES_UNIFORM 10
#define NUM_OF_TESTCASES_NORM 10
#define NUM_OF_TESTCASES_EXPONENTIAL 10
#define NUM_OF_TESTCASES_BERNOULLI 10
#define NUM_OF_TESTCASES_AES 4
#define NUM_OF_TESTCASES_CRC32 10

typedef struct {
    size_t n;
    uint32_t seed; 
} TestCaseBits;

typedef struct {
    size_t n;
    uint32_t seed; 
    float min;
    float max; 
} TestCaseUniform;

typedef struct {
    size_t n;
    uint32_t seed; 
    float mean;
    float stddev; 
} TestCaseNorm;

typedef struct {
    size_t n;
    uint32_t seed; 
    float lambda;
} TestCaseExponential;

typedef struct {
    size_t n;
    uint32_t seed; 
    float probability;
} TestCaseBernoulli;

typedef struct {
    float m1;
    float m2;
} SampleMoments;

SampleMoments get_sample_moments(uint32_t n, float *result);

typedef struct {
    size_t n;
    std::vector<uint8_t> key;
    std::vector<uint8_t> iv;
    std::vector<uint8_t> plaintext;
    std::vector<uint8_t> ciphertext;
    std::vector<uint8_t> tag;
} TestCaseAES;

typedef struct {
    std::string input;
    uint32_t result;
} TestCaseCRC32;

bool test_bits();
bool test_uniform();
bool test_norm();
bool test_exponential();
bool test_bernoulli();
bool test_aes256_gcm();
bool test_crc32();

int run_performance();
int run_correctness();
