// Tue  7 Dec 13:40:37 UTC 2021
// Mon  6 Dec 21:20:13 UTC 2021

#include <Arduino.h> // multi-file requires empty .ino and other .cpp - and this include
#include "ST7565.h"
#include "r_neo_pixel.h"

// stack ops/fifo  see:
//   https://github.com/wa1tnr/Metro-M4-Express-interpreter/blob/master/interpret_m4/interpret_m4.ino
const int STKSIZE = 8;
const int STKMASK = 7;
int stack[STKSIZE];
int p = 0;
#define TOS stack[p]

void prepend_to_fifo(int n) {
    p = (p - 1) & STKMASK;
    TOS = n;
}

// above:
// assign stack[p] to the passed value
// move the pointer backwards, modulo the stack size.

void push(int n) {
    prepend_to_fifo(n);
}

int consume_one_fifo_item(void) {
    int n = TOS;
    p = (p + 1) & STKMASK;
    return n;
}

// above:
// assign the return value from current contents of stack[p]
// move the pointer forward
// return the retrieved value

int pop(void) {
    int n = consume_one_fifo_item();
    return n;
}

volatile int glcd_busy = 0; // -1 is busy
volatile int sigA = 0;
volatile int sigA_cpy = 0;
volatile int sigB = 0;
volatile int sigB_cpy = 0;
volatile int sigSw = 0;

volatile int morseKludgeFlg = 0;

volatile uint8_t tick_recent, pbswitch_recent, neoPixel2_recent, lcd_clear_recent = 0; // FALSE

const uint8_t encoderKnobPinA = A1; // simple ints for D1 and D2 &c.  A1 only needed for the analog pins
const uint8_t encoderKnobPinB = A2;
const uint8_t pbSwitch = A3; // want interrupt on this pin, also.
const uint8_t backlight = A4; // controls NPN thru 47,000 ohms to base
const uint8_t LATCHSTATE = 3;
int8_t oldState = 3;

const int8_t KNOBDIR[] = {
    0, -1, 1, 0,
    1, 0, 0, -1,
    -1, 0, 0, 1,
    0, 1, -1, 0
};

volatile int positionInternal = 0;
volatile int positionExternal = 0;

// above: ROTARY ENCODER stuff

// below: LCD and GPS stuff

int ledPin = 13; // LED connected to digital pin 13
int buzzPin = A5;
/*
uint32_t timer = millis();
*/

ST7565 glcd(11, 10, 9, 6, 5); // good 4 Dec 2021

// definitions follow
void nopp(void) {
}; // no operation

void glcd_is_busy(void) {
    glcd_busy = -1; // TRUE
    // inhibit RotaryEncoder ISR
    detachInterrupt(encoderKnobPinA);
    detachInterrupt(encoderKnobPinB);
}

/* ISR - rotary shaft encoder - front panel control knob */
void tick_isr() {
    sigA = digitalRead(encoderKnobPinB);
    sigB = digitalRead(encoderKnobPinA);

    tick_recent = -1; // TRUE -- if FALSE here, the thing stalls out nicely
}

void glcd_not_busy(void) {
    glcd_busy = 0; // FALSE, go ahead, LCD doesn't need timing resources
    attachInterrupt(encoderKnobPinA, tick_isr, FALLING);
    attachInterrupt(encoderKnobPinB, tick_isr, FALLING);
}

void lcd_revision(void) {
    glcd_is_busy();
    glcd.drawstring(1, 1, "RTver 00-00l  13:40z");
    glcd.drawstring(1, 3, "             3f12dbb"); // previous commit
    glcd.drawstring(1, 3, " CHUPACABRA");
    glcd.drawstring(1, 5, " ra01k  c3q4"); // overwritten by:
    glcd.drawstring(1, 5, " bare rotary encoder");
    glcd.drawstring(1, 7, " FIFO exp ee 07DEC21 ");
    glcd.display();
    glcd.clear(); // use sparingly
    glcd_not_busy();
}

//  - - - - ------------------- ROTARY ENCODER ------------------

/* ISR - momentary pushbutton switch */
void sw_isr() { // ref: const uint8_t   pbSwitch = A3;
    sigSw = digitalRead(pbSwitch);
    pbswitch_recent = -1; // TRUE
}

// ISR above here

void setup_rotEnc(void) {
    pinMode(encoderKnobPinA, INPUT_PULLUP);
    pinMode(encoderKnobPinB, INPUT_PULLUP);
    pinMode(pbSwitch, INPUT_PULLUP);

    digitalWrite(encoderKnobPinA, HIGH); // raise the pin to Vcc
    digitalWrite(encoderKnobPinB, HIGH);
    digitalWrite(pbSwitch, HIGH);

    attachInterrupt(encoderKnobPinA, tick_isr, FALLING); // inverted dec 2021
    attachInterrupt(encoderKnobPinB, tick_isr, FALLING); // inverted dec 2021
    attachInterrupt(pbSwitch, sw_isr, FALLING);
}

void setup_LCD() {
    pinMode(backlight, OUTPUT);
    digitalWrite(backlight, LOW); // turn it off, brother
    glcd_is_busy();
    glcd.begin(0x18);
    glcd_not_busy();

    digitalWrite(backlight, HIGH); // turn it on, sister

    glcd_is_busy();
    glcd.display(); // show splashscreen
    glcd.clear();
    glcd_not_busy();
    delay(700);
    lcd_revision();
    delay(700);
    digitalWrite(backlight, LOW); // turn it off, brother

}

int bzFreqP = 180;
int bzFreqQ = 140;

void dah(void) { // low freq buzzer
    int braapDuration = 1100;
    for (int i = 0; i < braapDuration; i++) {
        digitalWrite(buzzPin, 1);
        delayMicroseconds(bzFreqP); // debounce
        digitalWrite(buzzPin, 0);
        delayMicroseconds(bzFreqQ); // debounce
    }
}

void dit(void) { // low freq buzzer
    int braapDuration = 400;
    for (int i = 0; i < braapDuration; i++) {
        digitalWrite(buzzPin, 1);
        delayMicroseconds(bzFreqP); // debounce

        digitalWrite(buzzPin, 0);
        delayMicroseconds(bzFreqQ); // debounce
    }
}

void m_ies(void) { // inter-element-space
    delayMicroseconds(153000);
}

void m_C(void) {
    dah();
    m_ies();
    dit();
    m_ies();
    dah();
    m_ies();
    dit();
}

void m_ics() {
    m_ies();
    m_ies();
    m_ies();
    m_ies();
    m_ies();
    m_ies();
}

void m_Q(void) {
    dah();
    m_ies();
    dah();
    m_ies();
    dit();
    m_ies();
    dah();
}

void morse(void) {
    dit();
    m_ies();
    dit();
}

void morse_rare(void) {
    dit();
    m_ies();
    dit();
    m_ies();
    dah();
    m_ies();
    dah();
    m_ies();
    dit();
    m_ies();
    dit();
}

void setup() {
    setup_neoPixel(); // club demo
    pinMode(buzzPin, OUTPUT); // club demo
    setup_rotEnc();
    setup_LCD();
    delay(200);
    /* KEEP THESE FOUR LINES 16:17z 06 Dec 2021: */
    if (0) {
        neoPixel(); // club demo
        morse();
    }
}

char bufferln[20] = "abcdefghijklmnopqrs";

void lcd_rot_multi_alts(void) {
    int col = 84;
    itoa(positionExternal, bufferln, DEC);

    glcd_is_busy();
    glcd.drawstring(col + 18, 3, "<< ");

    if (positionExternal > -1) {
        glcd.drawstring(col + 12, 3, bufferln);
        if (positionExternal == 0) {
            glcd.drawstring(col, 3, "  ");
        }
        if (positionExternal != 0) {
            glcd.drawstring(col, 3, " +");
        }
    }

    if (positionExternal < 0) {
        glcd.drawstring(col + 6, 3, bufferln);
        glcd.drawstring(col, 3, " -");
    }

    glcd.display(); // IMPORTANT CHANGE
    glcd_not_busy();
}

void lcd_rot_multi_3_to_9_alts(void) {
    int col = 84;
    int fake = 0;

    int position = positionExternal;

/*
    // trying to test default: a bit more thoroughly:
    glcd_is_busy();
    glcd.drawstring(col, 1, "    ");
    glcd_not_busy();
*/

    switch (position) {
    case 0:
      _meaningfulLabel_zed:

        glcd_is_busy();
        glcd.drawstring(col, 1, " *0*");
        glcd_not_busy();
        goto ending;

    case 1:
      _meaningfulLabel_one:
        glcd_is_busy();
        glcd.drawstring(col, 1, " *1*");
        glcd_not_busy();
        goto ending;

    case 2:
        glcd_is_busy();
        glcd.drawstring(col, 1, " *2*");
        glcd_not_busy();
        goto ending;

    case 3:
        glcd_is_busy();
        glcd.drawstring(col, 1, " &3&");
        glcd_not_busy();
        digitalWrite(backlight, LOW); // turn it off, brother
        goto ending;

// falls through to here if >3:

    case 4:
        glcd_is_busy();
        glcd.drawstring(col, 1, " ~4~");
        glcd_not_busy();
        digitalWrite(backlight, HIGH); // turn it on, sister
        goto ending;

    case 5:
        glcd_is_busy();
        glcd.drawstring(col, 1, " *5*");
        glcd_not_busy();
        morseKludgeFlg = -1;
        goto ending;

        // if (positionExternal >= 6) {

    case 6:
        glcd_is_busy();
        glcd.drawstring(col, 1, " #6#");
        glcd_not_busy();
        goto ending;

    case 7:
        glcd_is_busy();
        glcd.drawstring(col, 1, " *7*");
        glcd_not_busy();
        goto ending;

    case 8:
        glcd_is_busy();
        glcd.drawstring(col, 1, " ?8?");
        glcd_not_busy();
        if (morseKludgeFlg) {
            morse();
            morseKludgeFlg = 0;
        }
        goto ending;

    case 9:
        glcd_is_busy();
        glcd.drawstring(col, 1, " *9*");
        glcd_not_busy();
        goto ending;

    case 10:
        glcd_is_busy();
        glcd.drawstring(col, 1, "*10*");
        glcd_not_busy();
        goto ending;

    case 11:
        glcd_is_busy();
        glcd.drawstring(col, 1, "*HI*");
        glcd_not_busy();
        goto ending;

    case 12:
    case 13:
    case 14:
/*
            glcd_is_busy();
            glcd.drawstring(col, 1, "    ");
            glcd_not_busy();
*/
        goto ending;

    default:
        glcd_is_busy();
        glcd.drawstring(col, 1, "*QQ*");
        glcd_not_busy();
    } // switch

  ending:
    nopp();
}

int locked = 0; // FALSE
int lock_tick = 0; // count

void proc_tick(void) { // a 'tick' of the rotary shaft encoder signal train
    int8_t thisState = sigA_cpy | (sigB_cpy << 1);
    if (oldState != thisState) {

        positionInternal += KNOBDIR[thisState | (oldState << 2)];

        if (thisState == LATCHSTATE)
            positionExternal = positionInternal >> 2;

        oldState = thisState;
    }
}

void store_tick(void) { // FIFO stuff
    push(sigA);
    push(sigB);
}

void get_tick(void) {
    sigB_cpy = pop();
    sigA_cpy = pop();
}

void loop_for_rotEnc(void) { // if tick_recent  or  if pbswitch_recent

    if (tick_recent) {
        if (locked == 0) {
            // ignore tick when locked == -1
            store_tick(); // FIFO stuff

            get_tick(); // populate sigA_cpy and sigB_cpy

            proc_tick();

            lcd_rot_multi_3_to_9_alts();
        }
        tick_recent = 0; // reset tick_recent -- set the trap for a new acquisition
    }

    lcd_rot_multi_alts(); // not single-shot; do every loop

    if (pbswitch_recent) {
        pbswitch_recent = 0; // raise FALSE -- set the trap for a new acquisition
    }
}

void loop() // run over and over again
{
    if (tick_recent) {
        loop_for_rotEnc();
    }

/*
    if (timer > millis())
        timer = millis();

    if (millis() - timer > 1700) {
        timer = millis(); // reset the timer
        if (0) { }
    }
*/
}

// 4 dec 2021  20:46:28
// END
