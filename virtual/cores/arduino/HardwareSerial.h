#pragma once

#include "Stream.h"
#include <stdio.h>

class HardwareSerial : public Stream {
 public:
  HardwareSerial();
  void begin(unsigned long baud) {
    begin(baud, 0x06);
  }
  void begin(unsigned long, uint8_t);
  void end();
  virtual int available();
  virtual int peek();
  virtual int read();
  virtual int availableForWrite();
  virtual void flush();
  virtual size_t write(uint8_t);
  // we keep these four write()s the same as the default Arduino core
  inline size_t write(unsigned long n) {
    return write((uint8_t)n);
  }
  inline size_t write(long n) {
    return write((uint8_t)n);
  }
  inline size_t write(unsigned int n) {
    return write((uint8_t)n);
  }
  inline size_t write(int n) {
    return write((uint8_t)n);
  }
  using Print::write;  // write(str) and write(buf, size)
  operator bool() {
    return true;
  }
 private:
  static unsigned serialNumber;
  FILE* out;
};

class DebugStderrSerial : public HardwareSerial  {
  public:
  virtual size_t write(uint8_t);
  using Print::write;  // write(str) and write(buf, size)
};

// The default Arduino core only provides each of these HardwareSerial objects if
// various things are #defined.  We always provide them for virtual hardware.
extern HardwareSerial Serial;
#define HAVE_HWSERIAL0
extern HardwareSerial Serial1;
#define HAVE_HWSERIAL1
extern HardwareSerial Serial2;
#define HAVE_HWSERIAL2
extern HardwareSerial Serial3;
#define HAVE_HWSERIAL3

extern DebugStderrSerial& DebugStderr();


// end HardwareSerial

extern void serialEventRun(void) __attribute__((weak));

