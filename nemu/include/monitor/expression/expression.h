#include <stdio.h>
#include <string.h>

#define SIZE 100

char charStack[SIZE];
void charPop(void);
void charPush(char c);

int intStack[SIZE];
int intPop(void);
void intPush(int n);

char postfix[SIZE];			//后缀表达式
int top,postCount;

void createPostfixExpression(char *infix);		//利用中缀表达式生成后缀表达式
int calPostfixExpression(void);		//计算后缀表达式，返回结果
