// Mon  6 Dec 16:24:42 UTC 2021

#include <Arduino.h> // multi-file requires empty .ino and other .cpp - and this include
#include "ST7565.h"
#include "r_neo_pixel.h"

#define BACKLIGHT_LED 4
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
#define GPSSerial Serial1 // what's the name of the hardware serial port?
#define GPSECHO false

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
volatile int oldPositionExternal = 0;
volatile int hysteresis_upward = 0;

// above: ROTARY ENCODER stuff

// below: LCD and GPS stuff

int ledPin = 13; // LED connected to digital pin 13
int buzzPin = A5;
uint32_t timer = millis();

// Adafruit_GPS GPS(&GPSSerial);

ST7565 glcd(11, 10, 9, 6, 5); // good 4 Dec 2021

// definitions follow

void lcd_revision(void) {
    glcd.drawstring(1, 1, "RTver 00-00c  16:24z");
    glcd.drawstring(1, 3, "  CHUPACABRA");
    glcd.drawstring(1, 5, " ra01k  c3pa");
    glcd.drawstring(1, 5, " bare rotary encoder");
    glcd.drawstring(1, 7, " FIFO exp cc 06DEC21 ");
    glcd.display();
    glcd.clear(); // use sparingly
}

//  - - - - ------------------- ROTARY ENCODER ------------------

/* ISR - momentary pushbutton switch */
void sw_isr() { // ref: const uint8_t   pbSwitch = A3;
    sigSw = digitalRead(pbSwitch);
    pbswitch_recent = -1; // TRUE
}

/* ISR - rotary shaft encoder - front panel control knob */
void tick_isr() {
    sigA = digitalRead(encoderKnobPinB);
    sigB = digitalRead(encoderKnobPinA);

    tick_recent = -1; // TRUE -- if FALSE here, the thing stalls out nicely
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
    glcd.begin(0x18);

    digitalWrite(backlight, HIGH); // turn it on, sister

    glcd.display(); // show splashscreen
    glcd.clear();
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

    glcd.drawstring(col + 12, 3, bufferln);
    glcd.drawstring(col, 3, " -");
    glcd.drawstring(col + 18, 3, "Y- ");
    glcd.display(); // IMPORTANT CHANGE

}

void reset_positions(void) {
    positionInternal = 0;
    positionExternal = 0;
    oldPositionExternal = 0;
}

void decrement_positions(void) {
    positionInternal++;
    positionExternal++;
    oldPositionExternal++;
}

void increment_positions(void) {
    positionInternal++;
    positionExternal++;
    oldPositionExternal++;
}

void lcd_rot_multi_3_to_9_alts(void) {
    int col = 84;
    int fake = 0;

    glcd.drawstring(89, 7, "145.03"); // was " birds" leading space

    if (positionExternal == 3) {
        glcd.drawstring(col, 1, " &3&");
        digitalWrite(backlight, LOW); // turn it off, brother
    }
    if (positionExternal == 4) {
        glcd.drawstring(col, 1, " ~4~");
        digitalWrite(backlight, HIGH); // turn it on, sister
    }
    if (positionExternal == 5) {
        glcd.drawstring(col, 1, " *5*");
        morseKludgeFlg = -1;
    }

    if (positionExternal >= 6) {

        if (positionExternal == 6) {
            glcd.drawstring(col, 1, " #6#");
        }

        if (positionExternal == 8) {
            glcd.drawstring(col, 1, " ?8?");
            if (morseKludgeFlg) {
                morse();
                morseKludgeFlg = 0;
            }
        }
    }
}

int locked = 0; // FALSE
int lock_tick = 0; // count

void proc_sw(void) { // DO WE NEED PROCESS OR NOT
}

void proc_tick(void) { // a 'tick' of the rotary shaft encoder signal train
// volatile int sigA_cpy // volatile int sigB_cpy
    int8_t thisState = sigA_cpy | (sigB_cpy << 1);
    if (oldState != thisState) {

        positionInternal += KNOBDIR[thisState | (oldState << 2)];

        hysteresis_upward += KNOBDIR[thisState | (oldState << 2)];

        if (thisState == LATCHSTATE)
            positionExternal = positionInternal >> 2;

        oldState = thisState;
    }
    // wild guessin 4 dec 2021 17:38z KLUDGE:
}

void store_tick(void) { // FIFO stuff
    push(sigA);
    push(sigB);
}

void get_tick(void) {
// volatile int sigA_cpy // volatile int sigB_cpy
    sigB_cpy = pop();
    sigA_cpy = pop();
}

void loop_for_rotEnc(void) { // if tick_recent  or  if pbswitch_recent

    if (tick_recent) {
        if (locked == 0) {
            // ignore tick when locked == -1
            store_tick(); // FIFO stuff

            /*  WARNING   */// okay, well, advisory notice ;)

// proc_tick(); no longer is used but it should be used somewhere.  INTERIM development state.  KLUDGE.

            get_tick(); // populate sigA_cpy and sigB_cpy

            // proc_tick(); // if there was an interrupt recently, process the data in the globals sigA sigB
            proc_tick();

            lcd_rot_multi_3_to_9_alts();
        }
        tick_recent = 0; // reset tick_recent -- set the trap for a new acquisition
    }

    oldPositionExternal = positionExternal;
    lcd_rot_multi_alts(); // not single-shot; do every loop

    if (pbswitch_recent) {
        pbswitch_recent = 0; // raise FALSE -- set the trap for a new acquisition
    }
}

void loop() // run over and over again
{

    // char c = GPS.read();
    char c = 'A'; // kludge - was GPS.read();

    if (tick_recent) {
        loop_for_rotEnc();
    }

    if (timer > millis())
        timer = millis();

    if (millis() - timer > 1700) {
        timer = millis(); // reset the timer

        if (0) {
        }
    }
}

// 4 dec 2021  20:46:28
// END
