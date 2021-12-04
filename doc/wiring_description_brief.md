Sat  4 Dec 12:00:44 UTC 2021

The Feather GPS is stacked onto a Feather M0 Express underneath it.

Proper stacking headers were used (female above the board components; male pins passing through the copper clad holes at the edges per the design).

Feather M0 Express mounted on a smaller breadboard (old Radio Shack type).

Extra long pin headers used throughout, to make connections using Dupont style jumpers (like those used on older PC motherboards to accept items like reset switches, external speaker, HDD activity LED &c.).

A singleton green blinkie LED on a second Radio Shack breadboard (below the LCD; the Feather and GPS are above the LCD as mounted in the chassis).  It's (probably) on D4 or maybe A4 (don't remember).  Gets a current-limiting resistor.

Maybe the buzzer is on A5 - probably a straight DC interconnect (GPIO pin to buzzer to ground, no current-limiting resistor at all).

Rotary encoder was mounted on one or two small pieces of perfboard (if two, stacked, just for extra firm mechanicals).

Used a 'threading' technique to pass the wires from the (possibly) soldered connection (approx. five of them) to the main project, for mechanical stability.

 - - -

The wiring of the LCD is per the pinout in the photograph (see primary README of this project, at present - its reference may be documented elsewhere, later, but right now is in the main README.md).

A single current-limiting resistor was used inline with the Anode of the LCD backlight (which is in fact three separate LED's, electrically: Red, Green and Blue).

Each cathode is grounded individually, to turn that backlight color on, but only one is in use at any one time (change the jumper by hand).  Green is chosen for this reference design.  PWM could be used as well (to gain a wider color spectrum for the appearance of this LCD backlight).

An existing lib for the LCD (from the author's hard disk) was used, from 'back in the day' and may not be up to date.

 - - -

In December 2021, a current Arduino IDE compiler did complain about stuff with drawing strings on the LCD .. and there was junk on the screen that was a bit of a mystery.

memcpy() to the rescue - addresses this:

```
  -- snip --

 146 void lcd_revision(void) {
 147   char lcd_buffer[6];
 148   char* buffer_ptr;
 149   memcpy(lcd_buffer, "piper\000", sizeof lcd_buffer);
 150   buffer_ptr = lcd_buffer;
 151   glcd.drawstring(1, 1, buffer_ptr);
 152   glcd.display();
 153   glcd.clear(); // use sparingly
 154 }

  -- snip --
```

The extra NULL ('\000') was appended to the string literal (in double-quotes) as a consequence of empirical testing; without it, an extra junk character (a 'heart' char) was printed to the LCD.

The array index (here as '6') was carefully chosen as the minimum (storage fit for the literal "piper" plus an embedded NULL, following; for six characters in total, of required storage).

Further testing was not done (try a character array index value of 5 or 7, for comparison).

`'sizeof lcd_buffer'` can be captured and printed, for diagnostics and further understanding.

Future intent is to size correctly to this (very) LCD width (so '6' will become '16' maybe - don't recall the count of characters that fit the width).  Padding (ASCII 32, or 0x20 or \040) will be used, perhaps, to keep things simple.

Purists: the author apologizes here for winging it wrt the programming language employed.

Technique is entirely based on monkey-see, monkey-do - no effort was made to use the C/C++ language elements (or libraries) remotely 'properly'.

Instead, First Thing That Seems To Work Rule was applied, thoughout. ;)

END.
