#include "trap.h"

int add(int x, int y, int z) {
	return x+y+z;
}

int caller() {
	int temp1 = 125;
	int temp2 = 80;
	int temp3 = 40;
	int sum;
	set_bp();s
	sum = add(temp1, temp2, temp3);
	return sum;
}

int main() {
	caller();
	HIT_GOOD_TRAP;
	return 0;
}