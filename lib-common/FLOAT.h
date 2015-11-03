#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;
typedef struct floatStruct
{
	unsigned int tailCode		:23;
	unsigned int orderCode		:8;
	unsigned int sign		:1;
}FloatStruct;

static inline int F2int(FLOAT a) {
	int result = a>>16;
	return result;
}

static inline FLOAT int2F(int a) {
	FLOAT result = a<<16;
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

FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

#endif
