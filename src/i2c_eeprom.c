#include "ftd2xx.h"
#include "i2c_bitbang.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define AT24C256B_ADDR  0x50
#define EEPROM_BATCH_CAPCITY 0x800
#define EEPROM_DELAY  30

#define IS_INTEGER_CHAR(X) ((X)>=0x30 && (X)<=0x39)?1:(((X)>=0x41 && (X)<=0x5a)?1:(((X)>=0x61 && (X)<=0x7a)?1:0))

const char * help = "Usage: i2ce2prom I2CBUS ADDRESS BIT\n I2CBUS is an integer\n ADDRESS is an hex integer (0x03 - 0x77) BIT is 8 or 16\n";

int  BIT8_FLAG = 0;
void i2c_read(FT_HANDLE handle, uint16_t addr, uint8_t *dat,uint8_t ndat);
void i2c_write(FT_HANDLE handle, uint16_t addr,uint8_t dat);


void i2c_read(FT_HANDLE handle, uint16_t addr, uint8_t *dat,uint8_t ndat) {
  uint8_t serial[1024];
  uint32_t restart_point, nbytes;
  //start + addr + w +
  uint32_t len = i2c_begin(serial, AT24C256B_ADDR); //开始写 
  //高8位地址+
  len += i2c_send(serial + len,(addr>>8));
  //低8位地址+
  len += i2c_send(serial + len,(addr&0xff));
  //stop
  len += i2c_stop(serial + len);

  //start + addr + r +  接收 + stop
  restart_point = len;
  len += i2c_request(serial+len,AT24C256B_ADDR,ndat,1);
  
  FT_Write(handle, serial, len, (LPDWORD)&nbytes);
  FT_Read(handle, serial, nbytes, (LPDWORD)&nbytes);
  i2c_decode(serial + restart_point, dat, ndat);
}

void i2c_read_8bit(FT_HANDLE handle, uint8_t addr, uint8_t *dat,uint8_t ndat) {
  uint8_t serial[1024];
  uint32_t restart_point, nbytes;
  uint32_t len = i2c_begin(serial, addr);
  len += i2c_send(serial + len, addr);
  restart_point = len;
  len += i2c_request(serial + len, addr, ndat, 1);
  FT_Write(handle, serial, len, (LPDWORD)&nbytes);
  FT_Read(handle, serial, nbytes, (LPDWORD)&nbytes);
  i2c_decode(serial + restart_point, dat, ndat);
}

void i2c_write(FT_HANDLE handle, uint16_t addr,uint8_t dat)
{
  uint8_t serial[1024];
  uint32_t nbytes;       
  //发送I2C开始信号 start+
  uint32_t len = i2c_start(serial);
  //写地址  addr + w +
  len += i2c_send(serial + len, AT24C256B_ADDR<<1);  
  //发送高8位地址 high8bit+
  len += i2c_send(serial + len, addr>>8);
  //发送低8位地址 low8bit+
  len += i2c_send(serial + len, addr&0x0ff);
  //发送内容    content+
  len += i2c_send(serial + len,dat);
  //end session stop
  len += i2c_stop(serial + len);

   FT_Write(handle, serial, len, (LPDWORD)&nbytes);
   FT_Read(handle, serial, nbytes, (LPDWORD)&nbytes);
   Sleep(EEPROM_DELAY);
}

void e2prom_read(FT_HANDLE handle, uint16_t addr, uint8_t *dat,uint8_t ndat)
{
    i2c_read(handle,addr,dat,ndat);
}

void e2prom_write_8bit(FT_HANDLE handle, uint16_t addr, uint8_t* dat,uint16_t ndat)
{
    int index;
    for(index=0;index<ndat;index++)
      i2c_write(handle,addr+index,dat[index]);
}

void e2prom_write_16bit(FT_HANDLE handle, uint16_t addr, uint16_t dat)
{
    i2c_write(handle,addr,dat>>8);
    i2c_write(handle,addr+1,dat&0xff);
}

void e2prom_dump(FT_HANDLE handle, uint8_t *dat, uint16_t ndat)
{
 
  const uint8_t step = 0x10;
  
  for(int i=0x0000;i<ndat;i+=step){
   if(BIT8_FLAG)
      i2c_read_8bit(handle,i,dat+i,step);
    else
      i2c_read(handle,i,dat+i,step);
  }
 
}

void e2prom_erase(FT_HANDLE handle)
{
  for(int i=0;i<EEPROM_BATCH_CAPCITY;i++)
   i2c_write(handle,i,0xff);
}

void e2prom_dump_output(uint8_t *dat,uint16_t length)
{
  int vertical = 0,column = 0;
  printf("   ");
  for(vertical=0;vertical<0x10;vertical++)
    printf(" %2x",vertical);
  for(column=0;column<length;column++)
  {
    if(column%0x10 == 0){
      printf(" |");
      if(column != 0){
        for(int i=0;i<0x10;i++)
        {
          if(IS_INTEGER_CHAR(dat[column-0x10+i]))
          {
            printf("%c",dat[column-0x10+i]);
          }
          else
            printf("%c",0x2e);
        }
      }else{
        printf("0123456789abcdef");
      }
      printf("\n%03x",column/0x10);
    }
    printf(" %02x",dat[column]);
  }
  printf(" %02x",dat[column]);
  printf(" |");
  for(int i=0;i<0x10;i++)
  {
    if(IS_INTEGER_CHAR(dat[column-0x10+i]))
    {
      printf("%c",dat[column-0x10+i]);
    }
    else
      printf("%c",0x2e);
  }


}
  



int main(int argc, char *argv[])
{
  FT_STATUS ftStatus;
  FT_HANDLE handle;
  DWORD numDevs;

  int sel_dev = 0;
  int sel_bit = 0;
  uint8_t dat[1024]={0};
  uint8_t wDat[30] = {'h','e','l','l','o',' ','i','t',' ','i','s',' ','a',' ','e','e','p','r','o','m',' ','t','e','s','t',' '};
  uint16_t ndat = 0xff;

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
      sscanf(argv[3],"%d", &sel_bit);
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
  
  puts("eeprom write: 0xabed@0x0000");
  e2prom_write_16bit(handle, 0x0000,0xabed);
  puts("eeprom write: 0xeacc@0x0010");
  e2prom_write_16bit(handle, 0x0010, 0xeacc);

  if(sel_bit == 8)
     BIT8_FLAG = 1;
  else
  {
    BIT8_FLAG = 0;
  }
  

  clock_t start,finish;
  double duration;

  start = clock();
  // e2prom_write_16bit(handle,0x0000,0x1234);
  //e2prom_erase(handle);
  e2prom_write_8bit(handle,0x00,wDat,sizeof(wDat));
  e2prom_dump(handle,dat,ndat);
  e2prom_dump_output(dat,ndat);

  return 0;
}