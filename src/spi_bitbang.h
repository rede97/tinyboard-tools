#ifndef SPI_BITBANG_H
#define SPI_BITBANG_H

#include "stdint.h"

static const uint8_t SCL_H = 0b0001;
static const uint8_t SDO_H = 0b0010;
static const uint8_t SDI_H = 0b0100;

uint32_t spi_read(uint8_t *dst);
uint32_t spi_decoder(uint8_t *dst,uint8_t *dat);
#endif