#include "monitor/expression/expression.h"

void TokenPush(Token token)
{
	top++;
	TokenStack[top].type=token.type;
	strcpy(TokenStack[top].str,token.str);
}

void TokenPop(void)
{
	while(top>=0&&TokenStack[top].type!='(')
	{
		postfix[postCount].type=TokenStack[top].type;
		strcpy(postfix[postCount].str,TokenStack[top].str);
		top--;
		postCount++;
	}
}

void TokenPopOne(void)
{
	if(top>=0)
	{
		postfix[postCount].type=TokenStack[top].type;
		strcpy(postfix[postCount].str,TokenStack[top].str);
		top--;
		postCount++;
	}
}
	
void createPostfixExpression(Token *infix)
{
	int i;				//计数器
	int inCount=0;		//中缀表达式计数器
	top=-1;
	postCount=0;
	for(i=0;i<32;i++)
		postfix[i].type=0;	//初始化
	while(infix[inCount].type!=0)
	{
		switch(infix[inCount].type)
		{
		case NUM:
			postfix[postCount].type=infix[inCount].type;
			strcpy(postfix[postCount].str,infix[inCount].str);
			postCount++;
			break;
		case '+':case '-':
			TokenPop();
			TokenPush(infix[inCount]);
			break;
		case '*':
			if(0==inCount || (infix[inCount-1].type == '(') || (infix[inCount-1].type == '+') ||
				(infix[inCount-1].type == '-') || (infix[inCount-1].type == '*') ||
				(infix[inCount-1].type == '/'||infix[inCount-1].type == ADDR))	//如果在最开始，则为取地址，如果前面为运算符，则为取地址
			{
				infix[inCount].type=ADDR;
				TokenPush(infix[inCount]);
				break;
			}
			//其他情况作为乘号直接往下运行
		case '/':
			if(TokenStack[top].type=='+'||TokenStack[top].type=='-')
				TokenPush(infix[inCount]);
			else
			{
				while(TokenStack[top].type!='+'&&TokenStack[top].type!='-'&&top>=0)
					TokenPopOne();
				TokenPush(infix[inCount]);
			}
			break;
		case '(':
			TokenPush(infix[inCount]);
			break;
		case ')':
			TokenPop();
			top--;
			break;
		default:
			printf("illegal input!\n");
			postfix[0].type=0;
			return; 
		}
		inCount++;
	}
	TokenPop();
}

void intPush(int n)
{
	top++;
	intStack[top]=n;
}

int intPop(void)
{
	return intStack[top--];
}

int calPostfixExpression(void)
{
	int right_oprand;		//right_oprand为又操作数
	top=-1;
	postCount=0;		//初始化
	while(postfix[postCount].type!=0)
	{
		switch(postfix[postCount].type)
		{
		case NUM:
			intPush(atoi(postfix[postCount].str));
			break;
		case '+':
			right_oprand=intPop();
			intPush(intPop()+right_oprand);
			break;
		case '-':
			right_oprand=intPop();
			intPush(intPop()-right_oprand);
			break;
		case '*':
			right_oprand=intPop();
			intPush(intPop()*right_oprand);
			break;	
		case '/':
			right_oprand=intPop();
			if(right_oprand)
				intPush(intPop()/right_oprand);
			else
			{
				printf("divide zero\n");
				return -1;
			}
			break;
		case ADDR:
			intPush(swaddr_read(intPop(),4));
			break;
		default:
			printf("illegal input\n");
			return -1;
		}
		postCount++;
	}
	
	return intPop();
}
