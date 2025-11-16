#include <cstddef> 
#include <cstdint>
#include <cstring>
#include <vector>
#include <wmmintrin.h>

#include "solution.hpp"

static uint8_t gmul[256][256];
static uint8_t gmul_inv[256];
static uint8_t sbox[256];

static uint8_t gmul_impl(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        uint8_t hi = a & 0x80; // high bit
        a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

static uint8_t gf_inv(uint8_t x) {
    if (x == 0) return 0;
    uint8_t y = x;
    for (int i = 0; i < 6; i++) {
        y = gmul[y][y];
        y = gmul[y][x];
    }
    return gmul[y][y];
}

static uint8_t sbox_calc(uint8_t x) {
    x = gmul_inv[x];
    uint8_t s = x;
    for (int i = 0; i < 4; i++)
        s ^= (x = (x << 1) | (x >> 7));
    return s ^ 0x63;
}

static void gmul_precomp() {
#pragma omp parallel for
    for (int i = 0; i < 256 * 256; i++) {
        gmul[i / 256][i % 256] = gmul_impl(i / 256, i % 256);
    }

    for (int i = 0; i < 256; i++) {
        gmul_inv[i] = gf_inv(i);
    }

    for (int i = 0; i < 256; i++) {
        sbox[i] = sbox_calc(i);
    }
}

static uint8_t rcon(uint8_t i) {
    uint8_t c = 1;
    while (i-- > 1) c = gmul[c][2];
    return c;
}

static void key_expansion(const uint8_t *key, uint8_t *w) {
    memcpy(w, key, 32);
    
    for (int i = 8; i < 60; i++) {
        uint8_t *prev = w + (i - 1) * 4;
        uint8_t temp[4] = {prev[0], prev[1], prev[2], prev[3]};
        
        if (i % 8 == 0) {
            uint8_t t = temp[0];
            temp[0] = sbox[temp[1]] ^ rcon(i / 8);
            temp[1] = sbox[temp[2]];
            temp[2] = sbox[temp[3]];
            temp[3] = sbox[t];
        } else if (i % 8 == 4) {
            temp[0] = sbox[temp[0]];
            temp[1] = sbox[temp[1]];
            temp[2] = sbox[temp[2]];
            temp[3] = sbox[temp[3]];
        }
        
        uint8_t *base = w + (i - 8) * 4;
        w[i * 4 + 0] = base[0] ^ temp[0];
        w[i * 4 + 1] = base[1] ^ temp[1];
        w[i * 4 + 2] = base[2] ^ temp[2];
        w[i * 4 + 3] = base[3] ^ temp[3];
    }
}

void aes256_encrypt(const uint8_t *in, uint8_t *out, const uint8_t *key) {
    uint8_t w[240];
    key_expansion(key, w);

    __m128i state = _mm_loadu_si128((const __m128i*)in);
    state = _mm_xor_si128(state, *((const __m128i*)w));
    
    for (int round = 1; round < 14; round++) {
        state = _mm_aesenc_si128(state, *((const __m128i*)(w + round * 16)));
    }

    state = _mm_aesenclast_si128(state, *((const __m128i*)(w + 14 * 16)));
    _mm_storeu_si128((__m128i*)out, state);
}

static void gmul_block(const uint8_t *a, const uint8_t *b, uint8_t *result) {
    uint8_t v[16];
    uint8_t z[16] = {0};
    
    memcpy(v, b, 16);
    
    for (int i = 0; i < 16; i++) {
        for (int j = 7; j >= 0; j--) {
            if (a[i] & (1 << j)) {
                for (int k = 0; k < 16; k++) {
                    z[k] ^= v[k];
                }
            }

            uint8_t carry = 0;
            uint8_t next_carry;

            for (int k = 0; k < 16; k++) {
                next_carry = v[k] << 7;
                v[k] = (v[k] >> 1) | carry;
                carry = next_carry;
            }
            
            // Применяем редукцию по модулю, если был перенос
            if (carry) {
                v[0] ^= 0xE1;  // x^8 + x^4 + x^3 + x + 1 = 0x1B, но с учетом сдвига
            }
        }
    }
    
    memcpy(result, z, 16);
}

static void ghash(const uint8_t *h,
                  const uint8_t *ciphertext, size_t ciphertext_len, uint8_t *tag) {
    uint8_t x[16] = {0};
    uint8_t block[16];
    
    for (size_t i = 0; i < ciphertext_len; i += 16) {
        size_t block_len = (i + 16 <= ciphertext_len) ? 16 : ciphertext_len - i;
        memset(block, 0, 16);
        memcpy(block, ciphertext + i, block_len);
        
        for (int j = 0; j < 16; j++) {
            x[j] ^= block[j];
        }
        gmul_block(x, h, x);
    }
    
    uint64_t ciphertext_len_bits = ((uint64_t)ciphertext_len) * 8;
    
    memset(block, 0, 16);
    for (int i = 0; i < 8; i++) {
        block[15 - i] = (ciphertext_len_bits >> (i * 8)) & 0xff;
    }
    
    for (int j = 0; j < 16; j++) {
        x[j] ^= block[j];
    }
    gmul_block(x, h, x);
    
    memcpy(tag, x, 16);
}

static void add32(uint8_t *counter, uint32_t delta) {
    uint32_t val = ((uint32_t)counter[12] << 24) |
                   ((uint32_t)counter[13] << 16) |
                   ((uint32_t)counter[14] << 8) |
                   ((uint32_t)counter[15]);
    
    val += delta;
    
    counter[12] = (val >> 24) & 0xff;
    counter[13] = (val >> 16) & 0xff;
    counter[14] = (val >> 8) & 0xff;
    counter[15] = val & 0xff;
}

Status aes256_gcm(const uint8_t* plaintext, uint8_t* ciphertext,
                  const uint8_t* key, const uint8_t* iv, size_t plaintext_len, uint8_t* tag) {
    if (!key || !iv || !tag) {
        return STATUS_ERROR;
    }
    
    if (plaintext_len > 0 && (!plaintext || !ciphertext)) {
        return STATUS_ERROR;
    }
    
    gmul_precomp();

    uint8_t h[16] = {0};
    uint8_t zero_block[16] = {0};
    aes256_encrypt(zero_block, h, key);
    
    uint8_t j0[16];
    memset(j0, 0, 16);
    memcpy(j0, iv, 12);
    j0[15] = 0x01;
    
#pragma omp parallel for
    for (size_t i = 0; i < plaintext_len; i += 16) {
        uint8_t counter2[16];
        memcpy(counter2, j0, 16);
        add32(counter2, 1 + (i >> 4));

        uint8_t keystream[16];
        aes256_encrypt(counter2, keystream, key);
        
        size_t block_len = (i + 16 <= plaintext_len) ? 16 : plaintext_len - i;

        for (size_t j = 0; j < block_len; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ keystream[j];
        }
    }
    
    ghash(h, ciphertext, plaintext_len, tag);
    
    uint8_t e_j0[16];
    aes256_encrypt(j0, e_j0, key);

    for (int i = 0; i < 16; i++) {
        tag[i] ^= e_j0[i];
    }
    
    return STATUS_OK;
}
