#ifndef _CRC32_H
#define _CRC32_H

#include <cstdint>

#define crc32 crc32_lut

uint32_t crc32_lut(uint32_t crc, const void* msg, int size);
uint32_t crc32_soft(uint32_t crc, const void* msg, int size);

#endif
