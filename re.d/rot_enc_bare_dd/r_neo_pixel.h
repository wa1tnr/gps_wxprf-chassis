// Fri Jun  2 03:04:50 UTC 2017
// 4733-a6a

#ifndef NEO_PIXEL
#define NEO_PIXEL
// bring in any forth words you need, here:
// extern void dStack_push(cell_t value); // have to do this in every .h for every .cpp that calls dStack_push()
// extern cell_t dStack_pop(void);
// extern void _rot(void); // c_bz_eighth.cpp
// extern void _swap(void); // src/kernel/c_cf_ninth.cpp // x y -- y x

/* test

: magentaDull  14  0  14 rgb ;
: cyanDull 0 14 14 rgb ;
: greenBright 0 100 0 rgb ;

: ldelay 3000 delay ;
: red 75 0 0 rgb ;
: magenta  35  0  35 rgb ;
: cyan 0 19 19 rgb ;
: green 0 44 0 rgb ;
: blue 0 0 254 rgb ;
: orange 155 11 0 rgb ;
: yellow 75 35 0 rgb ;
: colours red ldelay green ldelay blue ldelay orange ldelay cyan ldelay magenta ldelay ;

: strobeonce pixel 2000 1000 * usDelay ;
: strobe strobeonce strobeonce strobeonce strobeonce ;

 */

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
// #define PIN            6

// How many NeoPixels are attached to the Arduino?
// #define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
// Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// int delayval = 500; // delay for half a second

extern void setup_neoPixel(void);
extern void neoPixel(void);
extern void neoPixel2(void);

#endif // #ifndef NEO_PIXEL
