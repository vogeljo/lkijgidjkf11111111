#pragma once
#include <string>
#include <cstdint>

uint32_t crc32(const void *buf, size_t size);
uint32_t crc32(uint32_t crc, const void *buf, size_t size);
uint32_t crc32(std::wstring str);