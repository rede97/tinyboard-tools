#include "ftd2xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

const uint8_t SCL_H = 0b0001;
const uint8_t SDA_H = 0b0100;
const uint8_t i2c_addr_start = 0x03, i2c_addr_stop = 0x77;

const uint8_t i2c_begin_serial_size = 30;
const uint8_t i2c_stop_serial_size = 3;

void print_wave(uint8_t *wav, uint32_t len, uint8_t bits) {
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

uint32_t i2c_begin(uint8_t *dst, uint8_t addr_wr) {
  dst[0] = SCL_H | SDA_H;
  dst[1] = SCL_H;
  dst[2] = 0x00;
  uint8_t *IIC_DATA = &dst[3];
  for (int i = 0; i < 8; ++i) {
    uint8_t bit = (addr_wr & 0x80) ? SDA_H : 0;
    IIC_DATA[i * 3] = bit;
    IIC_DATA[i * 3 + 1] = bit | SCL_H;
    IIC_DATA[i * 3 + 2] = bit;
    addr_wr <<= 1;
  }

  IIC_DATA[8 * 3] = SDA_H;
  IIC_DATA[8 * 3 + 1] = SDA_H | SCL_H;
  IIC_DATA[8 * 3 + 2] = SDA_H;

  return i2c_begin_serial_size;
}

uint32_t i2c_stop(uint8_t *dst) {
  dst[0] = 0x00;
  dst[1] = SCL_H;
  dst[2] = SCL_H | SDA_H;
  return i2c_stop_serial_size;
}

int8_t i2c_scan(FT_HANDLE fthandle, uint8_t *devices) {
  uint8_t i2c_scan_serial[(3 * 10 + 3) * 128];
  uint32_t idx = 0, nbytes = 0;
  uint32_t dev_cnt = 0;
  for (uint8_t addr = i2c_addr_start; addr <= i2c_addr_stop; ++addr) {
    idx += i2c_begin(i2c_scan_serial + idx, addr << 1);
    idx += i2c_stop(i2c_scan_serial + idx);
  }
  FT_Write(fthandle, i2c_scan_serial, idx, (LPDWORD)&nbytes);
  if (nbytes != idx) {
    return 0;
  }
  FT_Read(fthandle, i2c_scan_serial, nbytes, (LPDWORD)&nbytes);
  if (nbytes != idx) {
    return 0;
  }
  for (int i = 0; i <= i2c_addr_stop - i2c_addr_start; ++i) {
    if ((i2c_scan_serial[i * 33 + 29] & 0x02) == 0x00) {
      dev_cnt++;
      devices[i] = i2c_addr_start + i;
    } else {
      devices[i] = 0x00;
    }
  }
  return dev_cnt;
}

void i2c_detect_output(uint8_t * devices) {
  uint8_t i, addr;
  printf("   ");
  for (i = 0; i < 16; i++) {
    printf("%3x", i);
  }
  for (addr = 0; addr <= 119; addr++) {
    if (addr % 16 == 0) {
      printf("\n%02x:", addr & 0xF0);
    }
    if (addr >= i2c_addr_start && addr <= i2c_addr_stop) {
      if(devices[addr - i2c_addr_start] == 0) {
        printf(" --");
      } else {
        printf(" %02x", addr);
      }
    } else {
      printf("   ");
    }
  }
  printf("\n");
}

int main() {
  FT_STATUS ftStatus;
  FT_HANDLE handle;
  DWORD numDevs;

  ftStatus = FT_CreateDeviceInfoList(&numDevs);
  if (ftStatus == FT_OK) {
    printf("Number of FTDI devices is:%ld\n", numDevs);
    if (numDevs == 0) {
      return 0;
    }
  } else {
    return 1;
  }

  if (FT_Open(0, &handle)) {
    puts("Can't open device");
    return 1;
  }
  
  ftStatus = FT_SetBitMode(handle, 0x05, FT_BITMODE_SYNC_BITBANG);
  if (ftStatus) {
    return 1;
  }
  
  ftStatus = FT_SetBaudRate(handle, 38400);
  if (ftStatus) {
    return 1;
  }

  FT_Purge(handle, FT_PURGE_RX);
  uint8_t devices[128];
  i2c_scan(handle, devices);
  i2c_detect_output(devices);
  FT_Close(handle);
  return 0;
}