# tinyboard-tools
support i2c tools and spi tools etc.
>  Note: extrenal crystal or oscillator is necessary!

## Connect FT232RL with i2c device
![SCH](./img/sch_schem.png)

## Builld
1. Windows
```
>premake4 gmake
>make
```

## i2cdetect (GY-91)
```
PS ~> .\i2cdetect.exe 0
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

## i2cdump
```
PS ~> .\i2cdump.exe 0 0x68
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |0123456789abcdef|
00: c5 ce e0 0e a0 17 25 cf fa f0 fb fd 00 5a 5b 77 |......%......Z[w|
10: bf b9 a5 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
20: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
30: 00 00 00 00 00 00 00 00 00 00 05 c0 6c 01 4c 04 |............l.L.|
40: 84 12 70 ff 3f 00 00 00 13 00 00 00 00 00 00 00 |..p.?...........|
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
60: 00 00 01 00 00 00 00 00 00 00 00 01 00 00 00 00 |................|
70: 00 00 00 00 00 71 00 15 fa 00 eb 12 00 22 82 00 |.....q......."..|
80: c5 ce e0 0e a0 17 25 cf fa f0 fb fd 00 5a 5b 77 |......%......Z[w|
90: bf b9 a5 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
b0: 00 00 00 00 00 00 00 00 00 00 01 c0 90 01 8c 04 |................|
c0: 88 12 60 ff 3d 00 11 00 07 00 00 00 00 00 00 00 |..`.=...........|
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 |................|
e0: 00 00 00 00 00 00 00 00 00 00 00 01 00 00 02 84 |................|
f0: 00 00 00 00 00 71 00 15 fa 00 eb 12 00 22 82 00 |.....q......."..|
```
## i2ce2prom
```
λ eeprom_test.exe 0 0x50 16

erase page: 0x000
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |0123456789abcdef|
00: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
10: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
20: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
30: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
40: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
50: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
60: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
70: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
80: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
write page: "Hello World, EEPROM Write test" @0x0000
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f |0123456789abcdef|
00: 48 65 6c 6c 6f 20 57 6f 72 6c 64 2c 20 45 45 50 |Hello World, EEP|
10: 52 4f 4d 20 57 72 69 74 65 20 74 65 73 74 ff ff |ROM Write test..|
20: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
30: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
40: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
50: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
60: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
70: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|
80: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff |................|

```
