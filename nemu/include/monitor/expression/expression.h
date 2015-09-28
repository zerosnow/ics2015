#include "monitor/expr.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 20

Token TokenStack[SIZE];
void TokenPop(void);
void TokenPopOne(void);
void TokenPush(Token token);

int intStack[SIZE];
int intPop(void);
void intPush(int n);

Token postfix[32];			//后缀表达式
int top,postCount;

void createPostfixExpression(Token *infix);		//利用中缀表达式生成后缀表达式
int calPostfixExpression(void);		//计算后缀表达式，返回结果
