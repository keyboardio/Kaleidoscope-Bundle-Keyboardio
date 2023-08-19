#include "stdlib_ext.h"
#include <string.h>

// This implementation is adapted from en.wikibooks.org/wiki/C_Programming/stdlib.h/itoa
#define SIGNED_INT_TO_A \
  int i, sign; \
  if ((sign = val) < 0) val = -val; \
  i = 0; \
  do { \
    s[i++] = val % radix + '0'; \
  } while ((val /= 10) > 0); \
  if (sign < 0) \
    s[i++] = '-'; \
  s[i] = '\0'; \
  reverse(s);  /* reverse s in place */ \
  return s;

#define UNSIGNED_INT_TO_A \
  unsigned i = 0; \
  do { \
    s[i++] = val % radix + '0'; \
  } while ((val /= 10) > 0); \
  s[i] = '\0'; \
  reverse(s); \
  return s;

void reverse(char* s);  // reverse s in place

char* itoa(int val, char* s, int radix) {
  SIGNED_INT_TO_A
}

char* ltoa(long val, char* s, int radix) {
  SIGNED_INT_TO_A
}

char* utoa(unsigned int val, char* s, int radix) {
  UNSIGNED_INT_TO_A
}

char* ultoa(unsigned long val, char* s, int radix) {
  UNSIGNED_INT_TO_A
}

void reverse(char* s) {
  // from the same wikibooks link as itoa()
  int i, j;
  char c;
  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

// Could adapt from android.googlesource.com/toolchain/avr-libc/+/master/avr-libc-1.7.1/libc/stdlib/dtostre.c
// but this stub for now
char* dtostre(double val, char* s, unsigned char prec, unsigned char flags) {
  s[0] = '_';
  s[1] = '_';
  s[2] = '_';
  s[3] = '\0';
  return s;
}

char* dtostrf(double val, signed char width, unsigned char prec, char* s) {
  s[0] = '_';
  s[1] = '_';
  s[2] = '_';
  s[3] = '\0';
  return s;
}
