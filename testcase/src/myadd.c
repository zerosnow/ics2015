int add(int x, int y) {
	return x+y;
}

int caller() {
	int temp1 = 125;
	int temp2 = 80;
	int sum = add(temp1, temp2);
	return sum;
}

int main() {
	caller()
	HIT_GOOD_TRAP;
	return 0;
}