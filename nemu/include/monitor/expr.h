#ifndef __EXPR_H__
#define __EXPR_H__

#include "common.h"

uint32_t expr(char *, bool *);

enum {
	NOTYPE = 256, NUM, HEX, REG, ADDR, MINUS, EQU, UNEQU, AND, OR, NOT, SYMBOL
};

typedef struct token {
	int type;
	char str[32];
	int priority;
} Token;

#endif
