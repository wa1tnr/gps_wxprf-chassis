Sun  5 Dec 11:15:49 UTC 2021

The stack can be used as a FIFO, with care.  May not be
as efficient, but is simpler.

Instead of a push-down topology, the FIFO aspect can
be had by iterating (non-destructive) `pop();` one
less than the stack size (always an exact multiple
of two).

//   c.f.
//   https://github.com/wa1tnr/Metro-M4-Express-interpreter/blob/master/interpret_m4/interpret_m4.ino

```
int pop_back(void) {
    int value;
    for (index = STKMASK; index > 0; index--) {
        value = pop();
    }
    return value;
}
```

as would be found in program:

```
const int STKSIZE = 8;
const int STKMASK = 7;
int stack[STKSIZE];
int p = 0;
#define TOS stack[p]

void prepend_to_fifo(int n) {
  p = (p - 1)& STKMASK;
  TOS = n;
}

void push(int n) {
    prepend_to_fifo(n);
}

int consume_one_fifo_item(void) {
  int n = TOS;
  p = (p + 1)& STKMASK;
  return n;
}

int pop(void) {
    int n = consume_one_fifo_item();
    return n;
}

// .
// .
// functions including setup(); and loop();
// 5 dec 2021  11:21:49

```
