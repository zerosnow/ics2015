#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;

static inline int F2int(FLOAT a) {
	int result = a / 2^16;
	return result;
}

static inline FLOAT int2F(int a) {
	FLOAT result = a * 2^16;
	return result;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
	FLOAT result = a*b;
	return result;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
	FLOAT result = a/b;
	return result;
}

// FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

#endif
