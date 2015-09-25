#ifndef __EXPR_H__
#define __EXPR_H__

#include "common.h"

uint32_t expr(char *, bool *);

enum {
	NOTYPE = 256, NUM, HEX, REG, ADDR
};

typedef struct token {
	int type;
	char str[32];
} Token;

#endif
