# Recent
```
  Reworking the original program - probably not seen here.
  It is a box (weatherproof; the large Adafruit one) with:

    * GPS module
    * Adafruit Feather M0 Express
    * LCD ST7565 or somesuch - discontinued but have 3x on hand
    * Rotary encoder

  At the time (2017 or so) the idea was to use interrupts to
  somehow help the rotary encoder to overcome limitations.

  That is no longer viewed as an appropriate solution. ;)

  In addition, the object.drawstring() function from the Adafruit
  lib does not like string literals; the present program version
  (still private at this writing) is full of those and is in the
  process of being reworked, just to compile clean (no other goal).
```
```
 $  ls ~/foo/bar/Arduino/libraries/ST7565/
examples  glcdfont.c  ST7565.cpp  ST7565.h
```
```
 $  sha1sum  ~/Arduino/libraries/ST7565/*
sha1sum: /foo/bar/Arduino/libraries/ST7565/examples: Is a directory
97df9fdd7cf6d76ae56b545fcbd06faf04159c92  /foo/bar/Arduino/libraries/ST7565/glcdfont.c
76087ad24355de2479818e493aaf6cc7086925e8  /foo/bar/Arduino/libraries/ST7565/ST7565.cpp
56bb41618ec82ffe6594c5003ecbe72c54b55627  /foo/bar/Arduino/libraries/ST7565/ST7565.h
```
```
 $ git clone https://github.com/adafruit/ST7565-LCD.git
```  **not verified** - just a guess.

```
 $  find . | ag 7565 | cat -n
     1	./whollyunrelated
     2	./U8g2/src/clib/u8x8_d_st7565.c
     3	./ST7565
     4	./ST7565/glcdfont.c
     5	./ST7565/ST7565.h
     6	./ST7565/examples
     7	./ST7565/examples/st7565lcd
     8	./ST7565/examples/st7565lcd/st7565lcd.pde
     9	./ST7565/examples/gol
    10	./ST7565/examples/gol/gol.pde
    11	./ST7565/ST7565.cpp
``` _otoh .. kinda has to be?_

```
 $ cat ./main.cpp | ag 7565
#include "ST7565.h"
ST7565 glcd(11, 10, 9, 6, 5);
``` that's the call that 'works' in the program\*\* as coded.
\*\* the one you have not seen ;)

#### Thu  3 Apr 14:10:38 UTC 2025
