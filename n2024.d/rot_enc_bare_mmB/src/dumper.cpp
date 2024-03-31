#include <Arduino.h>

const int STKSIZEBB = 8;
const int STKMASKBB = 7;
int stackBB[STKSIZEBB];
int pBB = 0;
#define TOSBB stackBB[pBB]

int popBB() {
  int n = TOSBB;
  pBB = (pBB - 1)& STKMASKBB;
  return n;
}

void pushBB(int n) {
  pBB = (pBB + 1)& STKMASKBB;
  TOSBB = n;
}

void dumpRAM() {
  char bufferBB[5] = "";
  char *ram;
  int p = popBB();
  ram = (char*)p;
  sprintf(bufferBB, "%4x", p);
  Serial.print(bufferBB);
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    char c = *ram++;
    sprintf(bufferBB, " %2x", (c & 0xff));
    Serial.print(bufferBB);
  }
  ram = (char*)p;
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    bufferBB[0] = *ram++;
    if (bufferBB[0] > 0x7f || bufferBB[0] < ' ') bufferBB[0] = '.';
    bufferBB[1] = '\0';
    Serial.print(bufferBB);
  }
  pushBB(p + 16);
  Serial.println(); // kludge
}
