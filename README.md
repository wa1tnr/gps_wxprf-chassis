GPS in Weatherproof - Chassis code and mechanicals

Sat  4 Dec 11:45:17 UTC 2021

Wanted functional units to test individual hardware
enclosed in a single chassis.

Chassis already wired (2017) and tested on old
firmware.  This project aims to update and to
document to some extent. ;)


the chassis is here:

  [ https://www.adafruit.com/product/905 ]

the GPS module is here:

  [ https://www.adafruit.com/product/3133 ]

(Looks like it, anyway - could be another one from them but this is probably it)

the USB pass-through is here:

  [ https://www.adafruit.com/product/3258 ]

the buzzer (passive) is here:

  [ https://www.adafruit.com/product/160 ]

the graphic LCD is here:

  [ https://www.adafruit.com/product/250 ]

its pinout is here:

  [ https://cdn-shop.adafruit.com/970x728/250-04.jpg ]

It's wired to Feather M0 Express on the top row, D11-D5
  (contiguous, five pins exactly):

```
  -- snip --
 Adafruit_GPS GPS(&GPSSerial);
 134
 135
 136 // MOSI, SCK, A0, RST, CS
 137 // ST7565 glcd(11, 10, 9, 6, 5);
 138
 139 // Use MOSI and SCK hardware pins on D29 and D30:
 140 // ST7565 glcd(29, 30, 9, 6, 5);
 141 ST7565 glcd(11, 10, 9, 6, 5);  // stands as-is 4 Dec 2021

  -- snip --
```

Note: D29 D30 old news - corrected to D11 and D10 there.


END.
