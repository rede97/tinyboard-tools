#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

static uint32_t is_integer(char *s) {
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

static uint32_t is_hex(char *s) {
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

#endif