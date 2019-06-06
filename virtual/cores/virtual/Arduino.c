#include "Arduino.h"

// TODO: better time emulation
// this is pretty hacky, but hopefully helps most code behave sanely
// note: 'weak' attribute allows users to override with their own implementation of millis()
__attribute__((weak))
unsigned long millis(void) {
  static unsigned long time = 0;
  return time++;
}
unsigned long micros(void) {
  return millis()*1000;
}


// yes, these are pretty stupid with the current millis() and micros()
// but hopefully they stays fine if/when we get a better millis() and micros()

void delay(unsigned long ms) {
  unsigned long end = millis() + ms;
  while(millis() < end);
}

void delayMicroseconds(unsigned int us) {
  unsigned long end = micros() + us;
  while(micros() < end);
}
