#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>

static inline uint32_t is_integer(char *s) {
  uint32_t idx = 0;
  while (s[idx] != '\0') {
    if ((s[idx] >= '0') && (s[idx] <= '9')) {
      idx++;
    } else {
      return 0;
    }
  }
  return 1;
}

static inline uint32_t is_hex(char *s) {
  if ((s[0] == '0') && (s[1] == 'x')) {
    uint32_t idx = 2;
    while (s[idx] != '\0') {
      if (((s[idx] >= '0') && (s[idx] <= '9')) ||
          ((s[idx] >= 'a') && (s[idx] <= 'f'))) {
        idx++;
      } else {
        return 0;
      }
    }
    return 1;
  }
  return 0;
}

static inline void print_wave(uint8_t *wav, uint32_t len, uint8_t bits) {
  for (int j = 0; j < bits; ++j) {
    uint8_t last_bit = -1;
    for (int i = 0; i < len; ++i) {
      uint8_t bit = wav[i] >> j & 0x01;
      if (bit != last_bit) {
        putchar(bit + '0');
      } else {
        putchar('.');
      }
      last_bit = bit;
    }
    putchar('\n');
  }
}

#endif