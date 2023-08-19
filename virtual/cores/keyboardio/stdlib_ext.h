// These functions are defined in the avr stdlib.h, but not the standard one
// see descriptions in tools/avr/avr/include/stdlib.h

#ifdef __cplusplus
extern "C" {
#endif

char *itoa(int val, char* s, int radix);
char *ltoa(long val, char* s, int radix);
char *utoa(unsigned int val, char* s, int radix);
char *ultoa(unsigned long val, char* s, int radix);

char *dtostre(double val, char* s, unsigned char prec, unsigned char flags);
char *dtostrf(double val, signed char width, unsigned char prec, char* s);

#ifdef __cplusplus
}
#endif
