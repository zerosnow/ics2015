#ifndef __EXPR_H__
#define __EXPR_H__

#include "common.h"

uint32_t expr(char *, bool *);

typedef struct token {
	int type;
	char str[32];
} Token;

#endif
