#include "HardwareSerial.h"
#include "Arduino.h"

#undef min
#undef max

#include "Stream.h"
#include <stdio.h>
#include <iostream>


// see comments in the real HardwareSerial.cpp
void serialEvent() __attribute__((weak));
void serialEvent1() __attribute__((weak));
void serialEvent2() __attribute__((weak));
void serialEvent3() __attribute__((weak));
bool Serial0_available() __attribute__((weak));
bool Serial1_available() __attribute__((weak));
bool Serial2_available() __attribute__((weak));
bool Serial3_available() __attribute__((weak));

void serialEvent() {}
void serialEvent1() {}
void serialEvent2() {}
void serialEvent3() {}
bool Serial0_available() {
  return true;
}
bool Serial1_available() {
  return true;
}
bool Serial2_available() {
  return true;
}
bool Serial3_available() {
  return true;
}

void serialEventRun(void) {
  if (Serial0_available()) serialEvent();
  if (Serial1_available()) serialEvent1();
  if (Serial2_available()) serialEvent2();
  if (Serial3_available()) serialEvent3();
}

unsigned HardwareSerial::serialNumber = 0;

HardwareSerial::HardwareSerial() {}

void HardwareSerial::begin(unsigned long baud, byte config) {
  char filename[64];
  snprintf(filename, 64, "results/serial_%u.txt", serialNumber++);
  out = fopen(filename, "w");
}

void HardwareSerial::end() {
  if (out) fclose(out);
}

int HardwareSerial::availableForWrite(void) {
  return out ? 1000 : 0;
}
size_t HardwareSerial::write(uint8_t c) {
  if (out) fputc(c, out);
  return 1;
}
void HardwareSerial::flush(void) {
  if (out) fflush(out);
}

// TODO make input serial connections better.
// For now they're just essentially not allowed
int HardwareSerial::peek(void) {
  return -1;
}
int HardwareSerial::read(void) {
  return -1;
}
int HardwareSerial::available(void) {
  return 0;
}


size_t DebugStderrSerial::write(uint8_t c) {
   std::cerr << c ; 
  return 1;
}


HardwareSerial Serial;
HardwareSerial Serial1;
HardwareSerial Serial2;
HardwareSerial Serial3;

DebugStderrSerial DebugStderr;

