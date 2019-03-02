#include "spi_bitbang.h"
#include "windows.h"


#define SIMULATE_DELAY Sleep(3)

//CPOL=0,CPHA=0;
uint32_t spi_read(uint8_t *dst)
{
    int i;
    dst[0] = 0; 
    for(i=0;i<8;i++)
    {
        dst[2*i+1] = SCL_H;
        SIMULATE_DELAY;

        dst[2*i+2] = 0;
        SIMULATE_DELAY;        
    }

    return 0;
}

uint32_t spi_decoder(uint8_t *dst,uint8_t *dat)
{
    int i;
    for(i=0;i<8;i++)
    {
        if(dst[2*i+1] & SDI_H)
            *dat |= 0x01;
        else
        {
            *dat |= 0x00;
        }
        *dat << 1;
    }
    return 0;
}