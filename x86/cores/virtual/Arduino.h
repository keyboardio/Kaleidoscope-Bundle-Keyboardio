/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  Modified into main include file for the Kaleidoscope Virtual Keyboard
  Copyright (c) 2017 Craig Disselkoen.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "binary.h"
#include "stdlib_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

void yield(void);

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL1V1 2
#define INTERNAL INTERNAL1V1
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define DEFAULT 0
#define EXTERNAL 4
#define INTERNAL1V1 8
#define INTERNAL INTERNAL1V1
#define INTERNAL2V56 9
#define INTERNAL2V56_EXTCAP 13
#else
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
#define INTERNAL1V1 2
#define INTERNAL2V56 3
#else
#define INTERNAL 3
#endif
#define DEFAULT 1
#define EXTERNAL 0
#endif

// undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define interrupts() sei()
#define noInterrupts() cli()

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

typedef unsigned int word;

#define bit(b) (1UL << (b))

typedef bool boolean;
typedef uint8_t byte;

void init(void);
void initVariant(void);

void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
int analogRead(uint8_t);
void analogReference(uint8_t mode);
void analogWrite(uint8_t, int);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);

void attachInterrupt(uint8_t, void (*)(void), int mode);
void detachInterrupt(uint8_t);

void setup(void);
void loop(void);

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.

#define analogInPinToBit(P) (P)

// Ignore "PROGMEM" on virtual hardware
#define PROGMEM

// On the ATmega1280, the addresses of some of the port registers are
// greater than 255, so we can't store them in uint8_t's.
extern const uint16_t PROGMEM port_to_mode_PGM[];
extern const uint16_t PROGMEM port_to_input_PGM[];
extern const uint16_t PROGMEM port_to_output_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
// extern const uint8_t PROGMEM digital_pin_to_bit_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
//
// These perform slightly better as macros compared to inline functions
//
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_word( port_to_mode_PGM + (P))) )

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_AN_INTERRUPT -1

#ifdef ARDUINO_MAIN
#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6
#define PG 7
#define PH 8
#define PJ 10
#define PK 11
#define PL 12
#endif

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER1C 5
#define TIMER2  6
#define TIMER2A 7
#define TIMER2B 8

#define TIMER3A 9
#define TIMER3B 10
#define TIMER3C 11
#define TIMER4A 12
#define TIMER4B 13
#define TIMER4C 14
#define TIMER4D 15
#define TIMER5A 16
#define TIMER5B 17
#define TIMER5C 18

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"
#include "HardwareSerial.h"

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);

#endif  // __cplusplus

// We don't use avr/pgmspace.h, so the remainder of this file is a heavily abbreviated version
// of avr/pgmspace.h with the same or alternate definitions
// TODO: avr/pgmspace.h always includes avr/io.h, so do the same for avr/io.h
#include <inttypes.h>
#include <stddef.h>
#ifndef __ATTR_CONST__
#define __ATTR_CONST__ __attribute__((__const__))
#endif
#ifndef __ATTR_PROGMEM__
#define __ATTR_PROGMEM__ __attribute__((__progmem__))
#endif
#ifndef __ATTR_PURE__
#define __ATTR_PURE__ __attribute__((__pure__))
#endif
// We just discard the PROGMEM qualifier
#define PROGMEM


#ifndef PGM_P
#define PGM_P const char*
#endif
#ifndef PGM_VOID_P
#define PGM_VOID_P const void*
#endif

// We don't do anything special with PSTR
#define PSTR(s) ((const char*)(s))

#ifdef __cplusplus
// Not sure if these are acceptable substitute definitions in our context or not
inline uint8_t pgm_read_byte_near(const uint8_t *addr) { return *addr; }
inline int8_t pgm_read_byte_near(const int8_t *addr) { return *addr; }
inline char pgm_read_byte_near(const char *addr) { return *addr; }
inline uint16_t pgm_read_word_near(const uint16_t *addr) { return *addr; }
inline int16_t pgm_read_word_near(const int16_t *addr) { return *addr; }
inline uint32_t pgm_read_dword_near(const uint32_t *addr) { return *addr; }
inline int32_t pgm_read_dword_near(const int32_t *addr) { return *addr; }
inline float pgm_read_float_near(const float *addr) { return *addr; }
inline const void *pgm_read_ptr_near(const void **addr) { return *addr; }
#else
#define pgm_read_byte_near(addr) (*(const byte*)(addr))
#define pgm_read_word_near(addr) (*(const word*)(addr))
#define pgm_read_dword_near(addr) (*(const dword*)(addr))
#define pgm_read_float_near(addr) (*(const float*)(addr))
#define pgm_read_ptr_near(addr) (*(const void**)(addr))
#endif

#define pgm_read_byte_far(addr) pgm_read_byte_near(addr)
#define pgm_read_word_far(addr) pgm_read_word_near(addr)
#define pgm_read_dword_far(addr) pgm_read_dword_near(addr)
#define pgm_read_float_far(addr) pgm_read_float_near(addr)
#define pgm_read_ptr_far(addr) pgm_read_ptr_near(addr)
#define pgm_read_byte(addr) pgm_read_byte_near(addr)
#define pgm_read_word(addr) pgm_read_word_near(addr)
#define pgm_read_dword(addr) pgm_read_dword_near(addr)
#define pgm_read_float(addr) pgm_read_float_near(addr)
#define pgm_read_ptr(addr) pgm_read_ptr_near(addr)
#define pgm_get_far_address(var) (&(var))

#ifdef __cplusplus
extern "C" {
#endif

// _P or _PF string functions just call their standard library equivalents
#include <string.h>
#define memchr_P memchr
#define memcmp_P memcmp
#define memccpy_P memccpy
#define memcpy_P memcpy
#define memmem_P memmem
#define memrchr_P memrchr
#define strcat_P strcat
#define strchr_P strchr
#define strchrnul_P strchrnul
#define strcmp_P strcmp
#define strcpy_P strcpy
#define strcasecmp_P strcasecmp
#define strcasestr_P strcasestr
#define strcspn_P strcspn
#define strlcat_P strlcat
#define strlcpy_P strlcpy
#define strnlen_P strnlen
#define strncmp_P strncmp
#define strncasecmp_P strncasecmp
#define strncat_P strncat
#define strncpy_P strncpy
#define strpbrk_P strpbrk
#define strrchr_P strrchr
#define strsep_P strsep
#define strspn_P strspn
#define strstr_P strstr
#define strtok_P strtok
#define strtok_rP strtok_r
#define strlen_PF strlen
#define strnlen_PF strnlen
#define memcpy_PF memcpy
#define strcpy_PF strcpy
#define strncpy_PF strncpy
#define strcat_PF strcat
#define strlcat_PF strlcat
#define strncat_PF strncat
#define strcmp_PF strcmp
#define strncmp_PF strncmp
#define strcasecmp_PF strcasecmp
#define strncasecmp_PF strncasecmp
#define strstr_PF strstr
#define strlcpy_PF strlcpy
#define memcmp_PF memcmp
#define strlen_P strlen

#ifdef __cplusplus
}
#endif

#endif  // Arduino_h
