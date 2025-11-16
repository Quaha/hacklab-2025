#include <cstddef> 
#include <cstdint>
#include <cstring>
#include <vector>

#include "solution.hpp"

static uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        uint8_t hi = a & 0x80;
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
        y = gmul(y, y);
        y = gmul(y, x);
    }
    return gmul(y, y);
}

static uint8_t sbox_calc(uint8_t x) {
    x = gf_inv(x);
    uint8_t s = x;
    for (int i = 0; i < 4; i++)
        s ^= (x = (x << 1) | (x >> 7));
    return s ^ 0x63;
}

static void sub_bytes(uint8_t *s) {
    for (int i = 0; i < 16; i++)
        s[i] = sbox_calc(s[i]);
}

static void shift_rows(uint8_t *s) {
    uint8_t t;
    t = s[1]; s[1] = s[5]; s[5] = s[9]; s[9] = s[13]; s[13] = t;
    t = s[2]; s[2] = s[10]; s[10] = t;
    t = s[6]; s[6] = s[14]; s[14] = t;
    t = s[15]; s[15] = s[11]; s[11] = s[7]; s[7] = s[3]; s[3] = t;
}

static void mix_columns(uint8_t *s) {
    for (int i = 0; i < 4; i++) {
        uint8_t *c = s + i * 4;
        uint8_t a[4] = {c[0], c[1], c[2], c[3]};
        c[0] = gmul(a[0], 2) ^ gmul(a[1], 3) ^ a[2] ^ a[3];
        c[1] = a[0] ^ gmul(a[1], 2) ^ gmul(a[2], 3) ^ a[3];
        c[2] = a[0] ^ a[1] ^ gmul(a[2], 2) ^ gmul(a[3], 3);
        c[3] = gmul(a[0], 3) ^ a[1] ^ a[2] ^ gmul(a[3], 2);
    }
}

static void add_round_key(uint8_t *s, const uint8_t *k) {
    for (int i = 0; i < 16; i++) s[i] ^= k[i];
}

static uint8_t rcon(uint8_t i) {
    uint8_t c = 1;
    while (i-- > 1) c = gmul(c, 2);
    return c;
}

static void key_expansion(const uint8_t *key, uint8_t *w) {
    memcpy(w, key, 32);
    
    for (int i = 8; i < 60; i++) {
        uint8_t *prev = w + (i - 1) * 4;
        uint8_t temp[4] = {prev[0], prev[1], prev[2], prev[3]};
        
        if (i % 8 == 0) {
            uint8_t t = temp[0];
            temp[0] = sbox_calc(temp[1]) ^ rcon(i / 8);
            temp[1] = sbox_calc(temp[2]);
            temp[2] = sbox_calc(temp[3]);
            temp[3] = sbox_calc(t);
        } else if (i % 8 == 4) {
            temp[0] = sbox_calc(temp[0]);
            temp[1] = sbox_calc(temp[1]);
            temp[2] = sbox_calc(temp[2]);
            temp[3] = sbox_calc(temp[3]);
        }
        
        uint8_t *base = w + (i - 8) * 4;
        w[i * 4 + 0] = base[0] ^ temp[0];
        w[i * 4 + 1] = base[1] ^ temp[1];
        w[i * 4 + 2] = base[2] ^ temp[2];
        w[i * 4 + 3] = base[3] ^ temp[3];
    }
}

void aes256_encrypt(const uint8_t *in, uint8_t *out, const uint8_t *key) {
    uint8_t state[16], w[240];
    
    memcpy(state, in, 16);
    key_expansion(key, w);
    
    add_round_key(state, w);
    
    for (int round = 1; round < 14; round++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, w + round * 16);
    }
    
    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, w + 14 * 16);
    
    memcpy(out, state, 16);
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
            
            uint8_t carry = v[15] & 0x01;
            for (int k = 15; k > 0; k--) {
                v[k] = (v[k] >> 1) | (v[k-1] << 7);
            }
            v[0] = v[0] >> 1;
            
            if (carry) {
                v[0] ^= 0xE1;
            }
        }
    }
    
    memcpy(result, z, 16);
}

static void ghash(const uint8_t *h, const uint8_t *aad, size_t aad_len,
                  const uint8_t *ciphertext, size_t ciphertext_len, uint8_t *tag) {
    uint8_t x[16] = {0};
    uint8_t block[16];
    
    for (size_t i = 0; i < aad_len; i += 16) {
        size_t block_len = (i + 16 <= aad_len) ? 16 : aad_len - i;
        memset(block, 0, 16);
        memcpy(block, aad + i, block_len);
        
        for (int j = 0; j < 16; j++) {
            x[j] ^= block[j];
        }
        gmul_block(x, h, x);
    }
    
    for (size_t i = 0; i < ciphertext_len; i += 16) {
        size_t block_len = (i + 16 <= ciphertext_len) ? 16 : ciphertext_len - i;
        memset(block, 0, 16);
        memcpy(block, ciphertext + i, block_len);
        
        for (int j = 0; j < 16; j++) {
            x[j] ^= block[j];
        }
        gmul_block(x, h, x);
    }
    
    uint64_t aad_len_bits = ((uint64_t)aad_len) * 8;
    uint64_t ciphertext_len_bits = ((uint64_t)ciphertext_len) * 8;
    
    memset(block, 0, 16);
    for (int i = 0; i < 8; i++) {
        block[7 - i] = (aad_len_bits >> (i * 8)) & 0xff;
        block[15 - i] = (ciphertext_len_bits >> (i * 8)) & 0xff;
    }
    
    for (int j = 0; j < 16; j++) {
        x[j] ^= block[j];
    }
    gmul_block(x, h, x);
    
    memcpy(tag, x, 16);
}

static void inc32(uint8_t *counter) {
    uint32_t val = ((uint32_t)counter[12] << 24) |
                   ((uint32_t)counter[13] << 16) |
                   ((uint32_t)counter[14] << 8) |
                   ((uint32_t)counter[15]);
    
    val++;
    
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
    
    uint8_t h[16] = {0};
    uint8_t zero_block[16] = {0};
    aes256_encrypt(zero_block, h, key);
    
    uint8_t j0[16];
    if (12 == 12) {
        memset(j0, 0, 16);
        memcpy(j0, iv, 12);
        j0[15] = 0x01;
    } else {
        memset(j0, 0, 16);
        memcpy(j0, iv, 12);
        j0[15] = 0x01;
    }
    
    uint8_t counter[16];
    memcpy(counter, j0, 16);
    inc32(counter);
    
    for (size_t i = 0; i < plaintext_len; i += 16) {
        uint8_t keystream[16];
        aes256_encrypt(counter, keystream, key);
        inc32(counter);
        
        size_t block_len = (i + 16 <= plaintext_len) ? 16 : plaintext_len - i;
        for (size_t j = 0; j < block_len; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ keystream[j];
        }
    }
    
    ghash(h, nullptr, 0, ciphertext, plaintext_len, tag);
    
    uint8_t e_j0[16];
    aes256_encrypt(j0, e_j0, key);
    
    for (int i = 0; i < 16; i++) {
        tag[i] ^= e_j0[i];
    }
    
    return STATUS_OK;
}
