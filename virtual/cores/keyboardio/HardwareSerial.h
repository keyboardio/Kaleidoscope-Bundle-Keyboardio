#pragma once

#include "Stream.h"

#include <vector>
#include <queue>
#include <stdint.h>  // Use C-style header instead of <cstdint>
#include <stdio.h>

class HardwareSerial : public Stream {
 public:
  static constexpr size_t BUFFER_SIZE = 1024;
  static unsigned serialNumber;

  HardwareSerial();
  virtual ~HardwareSerial() = default;

  void begin(unsigned long baud) { begin(baud, 0x06); }
  void begin(unsigned long baud, uint8_t config);
  void end();

  // Arduino uses this to check if serial is ready
  operator bool() const { return true; }

  virtual int available(void) override;
  virtual int availableForWrite(void);
  virtual int peek(void) override;
  virtual int read(void) override;

  size_t readBytes(char *buffer, size_t length) {
    size_t count = 0;
    while (count < length && available()) {
      buffer[count++] = read();
    }
    return count;
  }

  virtual size_t write(uint8_t byte) override;
  virtual size_t write(const uint8_t *buffer, size_t size) override {
    for (size_t i = 0; i < size; i++) {
      write(buffer[i]);
    }
    return size;
  }

  virtual void flush(void) override;

  // Test helper methods
  void injectInput(const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; i++) {
      input_buffer_.push(data[i]);
    }
  }

  std::vector<uint8_t> getOutputBuffer() {
    std::vector<uint8_t> result;
    while (!output_buffer_.empty()) {
      result.push_back(output_buffer_.front());
      output_buffer_.pop();
    }
    return result;
  }

  void clearBuffers() {
    std::queue<uint8_t> empty;
    input_buffer_.swap(empty);
    output_buffer_.swap(empty);
  }

 private:
  std::queue<uint8_t> input_buffer_;
  std::queue<uint8_t> output_buffer_;
  FILE* out{nullptr};  // For file-based output
};

class DebugStderrSerial : public HardwareSerial  {
  public:
  virtual size_t write(uint8_t);
  using Print::write;  // write(str) and write(buf, size)
  operator bool() const { return true; }  // Always ready for debug output
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

