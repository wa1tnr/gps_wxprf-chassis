// Sat  4 Dec 18:04:39 UTC 2021

#include <Arduino.h> // multi-file requires empty .ino and other .cpp - and this include

// stack ops/fifo  see:
//   https://github.com/wa1tnr/Metro-M4-Express-interpreter/blob/master/interpret_m4/interpret_m4.ino
const int STKSIZE = 8;
const int STKMASK = 7;
int stack[STKSIZE];
int p = 0;
#define TOS stack[p]

void prepend_to_fifo(int n) {
  p = (p - 1)& STKMASK;
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
  p = (p + 1)& STKMASK;
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

// .
// .
// functions including setup(); and loop();
// 5 dec 2021  11:21:49
// END
