#include "monitor/expression/expression.h"
#include "memory/memory.h"

void TokenPush(Token token)
{
	top++;
	TokenStack[top].type=token.type;
	TokenStack[top].priority=token.priority;
	strcpy(TokenStack[top].str,token.str);
}

void TokenPop(void)
{
	while(top>=0&&TokenStack[top].type!='(')
	{
		postfix[postCount].type=TokenStack[top].type;
		postfix[postCount].priority=TokenStack[top].priority;
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
		postfix[postCount].priority=TokenStack[top].priority;
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
			postfix[postCount].priority=infix[inCount].priority;
			strcpy(postfix[postCount].str,infix[inCount].str);
			postCount++;
			break;
		case '+': case '-': case '*': case '/': case EQU: case UNEQU: case AND: case OR: 
			if(TokenStack[top].priority>infix[inCount].priority)
				TokenPush(infix[inCount]);
			else{
				while(TokenStack[top].priority<=infix[inCount].priority&&top>=0)
					TokenPopOne();
				TokenPush(infix[inCount]);
			}
			break;
		case ADDR: case MINUS: case NOT:
			TokenPush(infix[inCount]);
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
		case EQU:
			right_oprand=intPop();
			intPush(intPop()==right_oprand);
			break;
		case UNEQU:
			right_oprand=intPop();
			intPush(intPop()!=right_oprand);
			break;
		case AND:
			right_oprand=intPop();
			intPush(intPop()&&right_oprand);
			break;
		case OR:
			right_oprand=intPop();
			intPush(intPop()||right_oprand);
			break;
		case NOT:
			intPush(!intPop());
			break;
		case MINUS:
			intPush(0-intPop());
			break;
		case ADDR:
			right_oprand=intPop();
			right_oprand=right_oprand?((right_oprand-1)/4+1)*4:0;
			intPush(swaddr_read(right_oprand,4));
			break;
		default:
			printf("illegal input\n");
			return -1;
		}
		postCount++;
	}
	
	return intPop();
}
