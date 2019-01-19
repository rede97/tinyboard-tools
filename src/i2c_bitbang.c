#include "i2c_bitbang.h"

// void print_wave(uint8_t *wav, uint32_t len, uint8_t bits) {
//   for (int j = 0; j < bits; ++j) {
//     uint8_t last_bit = -1;
//     for (int i = 0; i < len; ++i) {
//       uint8_t bit = wav[i] >> j & 0x01;
//       if (bit != last_bit) {
//         putchar(bit + '0');
//       } else {
//         putchar('.');
//       }
//       last_bit = bit;
//     }
//     putchar('\n');
//   }
// }

uint32_t i2c_send(uint8_t *dst, uint8_t dat) {
  for (int i = 0; i < 8; ++i) {
    uint8_t bit = (dat & 0x80) ? SDA_H : 0;
    dst[i * 3] = bit;
    dst[i * 3 + 1] = bit | SCL_H;
    dst[i * 3 + 2] = bit;
    dat <<= 1;
  }

  // Wait for ack
  dst[8 * 3] = SDA_H;
  dst[8 * 3 + 1] = SDA_H | SCL_H;
  dst[8 * 3 + 2] = SDA_H;
  return i2c_send_serial_size;
}

uint32_t i2c_recv(uint8_t *dst, uint32_t ack) {
  for (int i = 0; i < 8; ++i) {
    dst[i * 3] = SDA_H;
    dst[i * 3 + 1] = SDA_H | SCL_H;
    dst[i * 3 + 2] = SDA_H;
  }

  if (ack) {
    dst[8 * 3] = 0x00;
    dst[8 * 3 + 1] = SCL_H;
    dst[8 * 3 + 2] = 0x00;
  } else {
    dst[8 * 3] = SDA_H;
    dst[8 * 3 + 1] = SDA_H | SCL_H;
    dst[8 * 3 + 2] = SDA_H;
  }
  return i2c_recv_serial_size;
}

uint32_t i2c_start(uint8_t *dst) {
  // SDA descending while SCL is HIGH.
  dst[0] = SCL_H | SDA_H;
  dst[1] = SCL_H;
  dst[2] = 0x00;
  return i2c_start_serial_size;
}

uint32_t i2c_stop(uint8_t *dst) {
  // SDA rasing while SCL is HIGH.
  dst[0] = 0x00;
  dst[1] = SCL_H;
  dst[2] = SCL_H | SDA_H;
  return i2c_stop_serial_size;
}

// Write Device
uint32_t i2c_begin(uint8_t *dst, uint8_t addr) {
  uint32_t len = i2c_start(dst);
  len += i2c_send(dst + len, addr << 1);
  return len;
}

// Read Device
uint32_t i2c_request(uint8_t *dst, uint32_t addr, uint32_t quantity,
                     uint32_t stop) {
  uint32_t len = i2c_start(dst);
  len += i2c_send(dst + len, (addr << 1) + 1);

  if (stop) {
    for (int i = 0; i < quantity - 1; ++i) {
      len += i2c_recv(dst + len, 1);
    }
    len += i2c_recv(dst + len, 0);
    len += i2c_stop(dst + len);
  } else {
    for (int i = 0; i < quantity; ++i) {
      len += i2c_recv(dst + len, 1);
    }
  }
  return len;
}

void i2c_decode(uint8_t *src, uint8_t *dst, uint32_t len) {
  src += i2c_start_serial_size + i2c_send_serial_size;
  for (int i = 0; i < len; ++i) {
    uint8_t v = 0x00;
    uint8_t *p = src + i2c_recv_serial_size * i;
    for (int j = 0; j < 8; ++j) {
      if (p[j * 3 + 1] & 0x02) {
        v = (v << 1) | 1;
      } else {
        v = (v << 1);
      }
    }
    dst[i] = v;
  }
}

