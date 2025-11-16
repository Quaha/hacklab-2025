#pragma once
#include <cstddef> 
#include <cstdint>

enum Status { STATUS_OK = 0, STATUS_ERROR = 1 };

/**
 * @brief Generates a sequence of pseudo-random bits using linear congruential generator.
 * 
 * @param n Number of random values to generate
 * @param seed Seed value for the random number generator
 * @param result Output array to store the generated random values
 * @return Status STATUS_OK on success, error code on failure
 */
Status generate_bits(size_t n, uint32_t seed, uint32_t* result);

/**
 * @brief Generates uniformly distributed random numbers in the specified range.
 * 
 * @param n Number of random values to generate
 * @param seed Seed value for the random number generator
 * @param min Minimum value of the uniform distribution
 * @param max Maximum value of the uniform distribution
 * @param result Output array to store the generated random values
 * @return Status STATUS_OK on success, error code on failure
 */
Status generate_uniform(size_t n, uint32_t seed, float min, float max, float* result);

/**
 * @brief Generates normally distributed random numbers using Box-Muller transform.
 * 
 * @param n Number of random values to generate
 * @param seed Seed value for the random number generator
 * @param mean Mean value of the normal distribution
 * @param stddev Standard deviation of the normal distribution
 * @param result Output array to store the generated random values
 * @return Status STATUS_OK on success, error code on failure
 */
Status generate_norm(size_t n, uint32_t seed, float mean, float stddev, float* result);

/**
 * @brief Generates exponentially distributed random numbers.
 * 
 * @param n Number of random values to generate
 * @param seed Seed value for the random number generator
 * @param lambda Rate parameter (lambda) of the exponential distribution
 * @param result Output array to store the generated random values
 * @return Status STATUS_OK on success, error code on failure
 */
Status generate_exponential(size_t n, uint32_t seed, float lambda, float* result);

/**
 * @brief Generates Bernoulli distributed random numbers (0 or 1).
 * 
 * @param n Number of random values to generate
 * @param seed Seed value for the random number generator
 * @param probability Probability of success (value 1) in range [0, 1]
 * @param result Output array to store the generated random values
 * @return Status STATUS_OK on success, error code on failure
 */
Status generate_bernoulli(size_t n, uint32_t seed, float probability, float* result);

/**
 * @brief Encrypts data using AES-256-GCM mode.
 * 
 * @param plaintext Input data to encrypt
 * @param ciphertext Output buffer for encrypted data (plaintext_len + 16 bytes for tag)
 * @param key 256-bit encryption key (32 bytes)
 * @param iv 96-bit initialization vector (12 bytes)
 * @param plaintext_len Length of plaintext data in bytes
 * @param tag Output buffer for authentication tag (16 bytes)
 * @return Status STATUS_OK on success, error code on failure
 */
Status aes256_gcm(const uint8_t* plaintext, uint8_t* ciphertext,
                  const uint8_t* key, const uint8_t* iv, size_t plaintext_len, uint8_t* tag);


/**
 * @brief Calculates CRC32 checksum for the given data.
 * 
 * @param data Input data buffer
 * @param data_len Length of data in bytes
 * @param result Output parameter to store the calculated CRC32 value
 * @return Status STATUS_OK on success, error code on failure
 */
Status crc32(const uint8_t* data, size_t data_len, uint32_t* result);
