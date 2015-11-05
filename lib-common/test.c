#include "FLOAT.h"
#include <stdio.h>

#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long A = a<0?-a:a;
	long long B = b<0?-b:b;
	long long temp = A*B;
	FLOAT result = (FLOAT)(temp>>16);
	if ((a < 0 &&  b > 0) || (a > 0 && b < 0)) result = -result;
	return result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	long long remain = a<0?-a:a;
	long long divisor = b<0?-b:b;
	int count;
	FLOAT result = 0;;
	remain = remain << 16;
	divisor = divisor << 16;
	count = 16;
	while(remain != 0) {
		if (remain >= divisor) {
			remain = remain - divisor;
			result = result | (1 << count);
		}
		if (count == 0) break;
		divisor = divisor >> 1;
		count --;
	}
	if ((a < 0 &&  b > 0) || (a > 0 && b < 0)) result = -result;
	return result;
}

FLOAT f2F(float a) {
	FLOAT result;
	int resultSign;
	int resultInteger;
	int resultFraction;
	FloatStruct *floatStruct = &a;
	unsigned int tailCode = floatStruct->tailCode & 0x007fffff;
	unsigned int orderCode = (floatStruct->orderCode & 0x000000ff)-127;
	unsigned int sign = floatStruct->sign & 0x1;
	if (orderCode >= -16 ||orderCode <= 14) {
		resultSign = sign;
		if (orderCode <= -1) {
			resultInteger = 0;
			//printf("%d\n", orderCode);
			resultFraction = tailCode >> (23-(16+orderCode)) | (1 << (16+orderCode));
		}
		else {
			resultInteger = 1 << orderCode | tailCode >> (23-orderCode) ;
			resultFraction = ((~(1 << 31)) >>(31-23+orderCode)) & 0xffff;
		}
		if (resultSign)
			result = resultSign << 31 | (~(resultInteger << 16 | resultFraction) +1);
		else
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



FLOAT f(FLOAT x) { 
	/* f(x) = 1/(1+25x^2) */
	return F_div_F(int2F(1), int2F(1) + F_mul_int(F_mul_F(x, x), 25));
}

FLOAT computeT(int n, FLOAT a, FLOAT b, FLOAT (*fun)(FLOAT)) {
	int k;
	FLOAT s,h;
	h = F_div_int((b - a), n);
	printf("%d, %d\n", fun(a), h);
	s = F_div_int(fun(a) + fun(b), 2 );

	printf("%d\n", s);
	for(k = 1; k < n; k ++) {
		s += fun(a + F_mul_int(h, k));
		printf("%d\n", s);
	}
	s = F_mul_F(s, h);
	return s;
}

int main() { 
	FLOAT a = computeT(10, f2F(-1.0), f2F(1.0), f);
	FLOAT ans = f2F(0.551222);
	printf("%d, %d\n", a, ans);

	printf("%d, %d\n",Fabs(a - ans),  f2F(1e-4) );
	//printf("%d, %d ,%d\n", f2F(30.0), f2F(0.3),F_div_F(f2F(30.0), f2F(0.3)));
	return 0;
}
