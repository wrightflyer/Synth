#ifndef _FAKELIBC_H_
#define _FAKELIBC_H_
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

char * 	fake_ltoa(long val, char *s, int radix);
char * 	fake_ultoa(unsigned long val, char *s, int radix);
char * 	fake_utoa(unsigned int val, char *s, int radix);
char * 	fake_itoa(int val, char *s, int radix);
char * 	dtostrf(double __val, signed char __width, unsigned char __prec, char *__s);

#endif // _FAKELIBC_H_