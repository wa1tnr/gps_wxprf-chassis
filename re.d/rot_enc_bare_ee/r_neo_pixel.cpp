// Fri Jun  2 03:04:50 UTC 2017
// 4733-a6a

// Adafruit original header, but this sketch operates only
// the one neopixel on the Feather M0 Express board.

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Arduino.h>
// #include "yaffa.h"
// #include "neo_pixel_CLUB.h"
// #include "r_neo_pixel_CLUB.h"
#include "r_neo_pixel.h"
// #include "Error_Codes.h"

#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
// #define PIN            6
#define PIN            8

// How many NeoPixels are attached to the Arduino?
// #define NUMPIXELS      16
#define NUMPIXELS      1

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels =
Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 300; // delay for half a second
int delaySval = 110; // delay for half a second

void setup_neoPixel(void) {
    pixels.begin(); // This initializes the NeoPixel library.
}

// crufted up here

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

// crufted down to here

void _rgbp(void) { // want them reversed for the idiom
    // _rot();
    // _rot();
    // _swap();
}

void _rgb(void) { // (  blue green red -- ) // CANNOT remember stack effect diagram order.

    // _rgbp(); // n3 n2 n1 -- n1 n2 n3

    // dStack_push(0); // blue
    // dStack_push(150); // green
    // dStack_push(0); // red
    // pixels.setPixelColor(i, pixels.Color(dStack_pop(), dStack_pop(), dStack_pop()));
    pixels.show();
}

void _cyan(void) {
    int i = 0; // this may be the neoPixel 'number' on a string of them -- we are zero
    // dStack_push(0); // red
    // dStack_push(19); // green
    // dStack_push(19); // blue
    pixels.setPixelColor(i, pixels.Color(0, 19, 19));
    _rgb();
}

void _orange(void) {
    int i = 0; // this may be the neoPixel 'number' on a string of them -- we are zero
    // dStack_push(155); // red
    // dStack_push(11); // green
    // dStack_push(0); // blue
    pixels.setPixelColor(i, pixels.Color(155, 11, 0));
    _rgb();
}

void _blue(void) {
    int i = 0; // this may be the neoPixel 'number' on a string of them -- we are zero
    // dStack_push(0); // red
    // dStack_push(0); // green
    // dStack_push(254); // blue
    pixels.setPixelColor(i, pixels.Color(0, 0, 254));
    _rgb();
}

void neoPixel(void) {
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
    for (int i = 0; i < NUMPIXELS; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(150, 0, 0)); // Red
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(delayval); // Delay for a period of time (in milliseconds).

        pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // Green
        pixels.show();
        delay(delayval);

        _orange();
        delay(delaySval);

        _blue();
        delay(delaySval);

        _orange();
        delay(delaySval);

        _blue();
        delay(delaySval);
        _cyan();

    }
}

void neoPixel2(void) {
    // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
    for (int i = 0; i < NUMPIXELS; i++) {
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255

        _orange();
        delay(delaySval);

        _blue();
        delay(delaySval);

        _orange();
        delay(delaySval);

        _blue();
        delay(delaySval);
        _cyan();

        _orange();
        delay(delaySval);

        _blue();
        delay(delaySval);
        _cyan();

        _orange();
        delay(delaySval);

        _blue();
        delay(delaySval);
        _cyan();

    }
}

extern const char pixel_str[]; // = "pixel"; // local idiom yainsu
void _pixel(void) {
    neoPixel();
}
