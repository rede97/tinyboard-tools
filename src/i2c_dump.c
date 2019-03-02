#include "ftd2xx.h"
#include "i2c_bitbang.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void i2c_read_reg(FT_HANDLE handle, uint8_t addr, uint8_t reg, uint8_t *dat,
                  uint8_t ndat) {
  uint8_t serial[1024];
  uint32_t restart_point, nbytes;
  uint32_t len = i2c_begin(serial, addr);
  len += i2c_send(serial + len, reg);
  restart_point = len;
  len += i2c_request(serial + len, addr, ndat, 1);
  FT_Write(handle, serial, len, (LPDWORD)&nbytes);
  FT_Read(handle, serial, nbytes, (LPDWORD)&nbytes);
  i2c_decode(serial + restart_point, dat, ndat);
}

void i2c_dump(FT_HANDLE handle, uint8_t addr, uint8_t *dat) {
  const uint8_t step = 0x10;
  for (int i = 0x00; i < 0xff; i += step) {
    i2c_read_reg(handle, addr, i, dat + i, step);
  }
}

void i2c_dump_output(uint8_t *dat) {
  char str[17] = {0};
  printf("   ");
  for (int j = 0; j < 0x10; ++j) {
    printf(" %2x", j);
  }
  printf(" |0123456789abcdef|");
  
  for (int i = 0; i < 0xff; i += 0x10) {
    printf("\n%02x:", i);
    hex2str(&dat[i], str, 0x10);
    for (int j = 0; j < 0x10; ++j) {
      printf(" %02x", dat[i + j]);
    }
    printf(" |%s|", str);
  }
}

const char * help = "Usage: i2cdump I2CBUS ADDRESS\n I2CBUS is an integer\n ADDRESS is an hex integer (0x03 - 0x77)\n";

int main(int argc, char *argv[]) {

  FT_STATUS ftStatus;
  FT_HANDLE handle;
  DWORD numDevs;

  int sel_dev = 0;
  uint8_t addr;

  ftStatus = FT_CreateDeviceInfoList(&numDevs);
  if (ftStatus != FT_OK) {
    printf("Can not create device info list.");
    return 1;
  }
  if (numDevs == 0) {
    printf("Not find any FTDI device.");
    return 0;
  }

  if (argc != 1) {
    if (is_integer(argv[1]) && is_hex(argv[2])) {
      sscanf(argv[1], "%d", &sel_dev);
      uint32_t input_hex;
      sscanf(argv[2], "%x", &input_hex);
      if(input_hex < 0x03 || input_hex > 0x77) {
        puts(help);
        return 0;
      } else {
        addr = input_hex;
      }
    } else {
      puts(help);
      return 0;
    }
  } else {
    puts(help);
    return 0;
  }

  if (FT_Open(sel_dev, &handle)) {
    puts("Can't open device");
    return 1;
  }

  ftStatus = FT_SetBitMode(handle, 0x03, FT_BITMODE_SYNC_BITBANG);
  if (ftStatus) {
    return 1;
  }

  ftStatus = FT_SetBaudRate(handle, 9600);
  if (ftStatus) {
    return 1;
  }

  FT_Purge(handle, FT_PURGE_RX);

  uint8_t dat[512] = {0};
  i2c_dump(handle, addr, dat);
  i2c_dump_output(dat);
  FT_Close(handle);
  return 0;
}
