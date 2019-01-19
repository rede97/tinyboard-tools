# tinyboard-tools
support i2c tools and spi tools etc.

## i2c-detect
1. Connect FT232RL with mpu9250(GY-91)

TX(D0, output) genetate the clock for SCK in i2c bus, RX(D1, input) read the voltage signal of SDA, CTS(D2, output) would pull the SDA down to 200mV if you set CTS with digital 0. SDA pull up with 4.7Kohm resistor by default.

2. Result of scan i2c bus
```
PS ~> .\i2c-detect.exe
Number of FTDI devices is:1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- 68 -- -- -- -- -- -- --
70: -- -- -- -- -- -- 76 --
```
