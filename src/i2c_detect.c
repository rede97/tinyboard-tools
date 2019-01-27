#include "ftd2xx.h"
#include "i2c_bitbang.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int8_t i2c_detect(FT_HANDLE fthandle, uint8_t *devices) {
  uint8_t i2c_scan_serial[(3 * 10 + 3) * 128];
  uint32_t idx = 0, nbytes = 0;
  uint32_t dev_cnt = 0;
  for (uint8_t addr = i2c_addr_start; addr <= i2c_addr_stop; ++addr) {
    idx += i2c_begin(i2c_scan_serial + idx, addr);
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
    if ((i2c_scan_serial[i * 33 + 29] & SDA_H) == 0x00) {
      dev_cnt++;
      devices[i] = i2c_addr_start + i;
    } else {
      devices[i] = 0x00;
    }
  }
  return dev_cnt;
}

void i2c_detect_output(uint8_t *devices) {
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
      if (devices[addr - i2c_addr_start] == 0) {
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

const char *help = "Usage: i2cdetect I2CBUS\n i2cdetect -l\n I2CBUS is an "
                   "integer or an I2C bus name\n";

int main(int argc, char *argv[]) {

  FT_STATUS ftStatus;
  FT_HANDLE handle;
  DWORD numDevs;

  int sel_dev = 0;

  ftStatus = FT_CreateDeviceInfoList(&numDevs);
  if (ftStatus != FT_OK) {
    printf("Can't create device info list.");
    return 1;
  }
  if (numDevs == 0) {
    printf("Not find any FTDI device.");
    return 0;
  }

  if (argc != 1) {
    if (argv[1][0] == '-') {
      if (argv[1][1] == 'l') {
        FT_DEVICE_LIST_INFO_NODE *devInfo = (FT_DEVICE_LIST_INFO_NODE *)malloc(
            sizeof(FT_DEVICE_LIST_INFO_NODE) *
            numDevs); // get the device information list
        ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
        if (ftStatus == FT_OK) {
          for (int i = 0; i < numDevs; i++) {
            printf("dev %d:\t", i);
            printf("%s\n", devInfo[i].Description);
          }
        }
        return 0;
      } else {
        puts(help);
        return 0;
      }
    } else {
      if (is_integer(argv[1])) {
        sscanf(argv[1], "%d", &sel_dev);
      } else {
        puts(help);
        return 0;
      }
    }
  }

  if (FT_Open(sel_dev, &handle)) {
    puts("Can't open device.");
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
  uint8_t devices[128];
  i2c_detect(handle, devices);
  i2c_detect_output(devices);
  FT_Close(handle);
  return 0;
}
