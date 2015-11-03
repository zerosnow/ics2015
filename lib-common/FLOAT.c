#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	FLOAT result = (a>>8)*(b>>8);
	return result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	FLOAT result = (a<<8) / (b>>8);
	return result;
}

FLOAT f2F(float a) {
	FLOAT result;
	int resultSign;
	int resultInteger;
	int resultFraction;
	FloatStruct *floatStruct = &a;
	unsigned int tailCode = floatStruct->tailCode & 0x007fffff;
	unsigned int orderCode = floatStruct->orderCode & 0x000000ff-127;
	unsigned int sign = floatStruct->sign & 0x1;
	if (orderCode >= -16 ||orderCode <= 14) {
		resultSign = sign;
		if (orderCode <= -1) {
			resultInteger = 0;
			resultFraction = tailCode >> (23-(16+orderCode)) | (1 << (16+orderCode));
		}
		else {
			resultInteger = 1 << orderCode | tailCode >> (23-orderCode) ;
			resultFraction = ((~(1 << 31)) >>(31-23+orderCode)) & 0xffff;
		}
		result = resultSign << 31 | resultInteger << 16 | resultFraction;
	}
	else 
		result = 0;
	//printf("%d, %d, %d\n", floatStruct->tailCode, floatStruct->orderCode, floatStruct->sign);
	return result;
}

FLOAT Fabs(FLOAT a) {
	FLOAT result = a<0?-a:a;
	return result;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

