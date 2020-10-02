#include "fakelibc.h"

char * 	fake_ltoa(long val, char *s, int radix) {
	sprintf(s, "%ld", val);
	return s;
}

char * 	fake_ultoa(unsigned long val, char *s, int radix) {
	sprintf(s, "%lu", val);
	return s;
}

char * 	fake_utoa(unsigned int val, char *s, int radix) {
	sprintf(s, "%u", val);
	return s;
}

char * 	fake_itoa(int val, char *s, int radix) {
	sprintf(s, "%d", val);
	return s;
}

char * 	dtostrf(double __val, signed char __width, unsigned char __prec, char *__s) {
	sprintf(__s, "%f", __val);
	return __s;
}