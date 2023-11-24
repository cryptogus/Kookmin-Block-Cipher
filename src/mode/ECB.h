#pragma once

#include <stdint.h>
#include <string.h>

void ECB(void (*cipher)(uint8_t *, uint8_t *, uint8_t *) , uint8_t *key, int block_size, size_t len, unsigned char *src, unsigned char *des);
