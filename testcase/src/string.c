#include "trap.h"
#include <string.h>

char *s[] = {
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",
	"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
	", World!\n",
	"Hello, World!\n",
	"#####"
};

char str1[] = "Hello";
char str[20];

int main() {
	//nemu_assert(strcmp(s[0], s[2]) == 0);
	//nemu_assert(strcmp(s[0], s[1]) == -1);
	//nemu_assert(strcmp(s[0] + 1, s[1] + 1) == -1);
	//nemu_assert(strcmp(s[0] + 2, s[1] + 2) == -1);
	//nemu_assert(strcmp(s[0] + 3, s[1] + 3) == -1);
	char *ss = strcat(strcpy(str, str1), s[3]);
	nemu_assert(ss[0] == 'H');
	nemu_assert(ss[1] == 'e');
	nemu_assert(ss[2] == 'l');
	nemu_assert(ss[3] == 'l');
	nemu_assert(ss[4] == 'o');
	nemu_assert(ss[5] == ',');
	nemu_assert(ss[6] == ' ');
	nemu_assert(ss[7] == 'W');
	nemu_assert(ss[8] == 'o');
	nemu_assert(ss[9] == 'r');
	nemu_assert(ss[10] == 'l');
	nemu_assert(ss[11] == 'd');
	nemu_assert(ss[12] == '!');
	nemu_assert(ss[13] == '\n');
	nemu_assert(ss[14] == '\0');

	//nemu_assert(strcmp( strcat(strcpy(str, str1), s[3]), s[4]) == 0);
	//nemu_assert(strcmp(strcpy(str, str1), str1) == 0);
	//nemu_assert(strcmp(strcat(str1, s[3]), s[4]) == 0);
	//nemu_assert(memcmp(memset(str, '#', 5), s[5], 5) == 0);

	HIT_GOOD_TRAP;
	return 0;
}
