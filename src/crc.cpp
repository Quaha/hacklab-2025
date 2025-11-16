#include <cstddef> 
#include <cstdint>
#include <cmath>

#include "solution.hpp"

Status crc32(const uint8_t* data, size_t data_len, uint32_t* result) {
    uint32_t crc = 0xFFFFFFFF;
    
    for (size_t i = 0; i < data_len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
            crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
    }

    *result = ~crc;
    
    return STATUS_OK;
}
