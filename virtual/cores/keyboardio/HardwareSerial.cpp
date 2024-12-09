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
  // Nothing needed for virtual serial
}

void HardwareSerial::end() {
  clearBuffers();
}

int HardwareSerial::availableForWrite(void) {
  return BUFFER_SIZE - output_buffer_.size();
}

size_t HardwareSerial::write(uint8_t c) {
  if (output_buffer_.size() < BUFFER_SIZE) {
    output_buffer_.push(c);
    return 1;
  }
  return 0;
}

void HardwareSerial::flush(void) {
  // Nothing needed for virtual serial
}

int HardwareSerial::peek(void) {
  if (input_buffer_.empty()) {
    return -1;
  }
  return input_buffer_.front();
}

int HardwareSerial::read(void) {
  if (input_buffer_.empty()) {
    return -1;
  }
  uint8_t c = input_buffer_.front();
  input_buffer_.pop();
  return c;
}

int HardwareSerial::available(void) {
  return input_buffer_.size();
}

size_t DebugStderrSerial::write(uint8_t c) {
  std::cerr << c;
  return 1;
}

HardwareSerial Serial;
HardwareSerial Serial1;
HardwareSerial Serial2;
HardwareSerial Serial3;

DebugStderrSerial& DebugStderr() {
  static DebugStderrSerial* ans = new DebugStderrSerial();
  return *ans;
}

