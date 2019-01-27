#ifndef I2C_BITBANG_H
#define I2C_BITBANG_H

#include <stdint.h>

static const uint8_t SCL_H = 0b0001;
static const uint8_t SDA_H = 0b0010;
static const uint8_t i2c_addr_start = 0x03, i2c_addr_stop = 0x77;

static const uint32_t i2c_start_serial_size = 3;
static const uint32_t i2c_stop_serial_size = 3;
static const uint32_t i2c_send_serial_size = 24 + 3;
static const uint32_t i2c_recv_serial_size = 24 + 3;

uint32_t i2c_send(uint8_t *dst, uint8_t dat);
uint32_t i2c_recv(uint8_t *dst, uint32_t ack);

uint32_t i2c_begin(uint8_t *dst, uint8_t addr);
uint32_t i2c_stop(uint8_t *dst);
uint32_t i2c_request(uint8_t *dst, uint32_t addr, uint32_t quantity,
                     uint32_t stop);

void i2c_decode(uint8_t *src, uint8_t *dst, uint32_t len);

#endif