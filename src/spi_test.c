#include "ftd2xx.h"
#include "spi_bitbang.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

const char * help = "Usage: i2cdump I2CBUS ADDRESS\n I2CBUS is an integer\n ADDRESS is an hex integer (0x03 - 0x77)\n";

int main(int argc, char *argv[])
{
  FT_STATUS ftStatus;
  FT_HANDLE handle;
  DWORD numDevs;
  int sel_dev = 0;

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


  uint8_t dst[1024]={0};
  uint8_t dat=0;

  uint32_t len = spi_read(dst);
  uint32_t nbytes;

  FT_Write(handle, dst, len, (LPDWORD)&nbytes);
  FT_Read(handle, dst, nbytes, (LPDWORD)&nbytes);

  spi_decoder(dst,&dat);



  return 0;
}