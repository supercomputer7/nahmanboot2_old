#pragma once

#include <stdint.h>
#include <LibC/stdbool.h>

bool strncmp(const char *str1, const char *str2,uint16_t len);
void strcopy(char *source, char *dest,uint32_t len);
void memset(uint8_t* source,uint8_t value,uint32_t len);
uint32_t strlen(const char *str);