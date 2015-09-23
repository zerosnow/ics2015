#include "monitor/expression/expression.h"

void charPush(char c)
{
	top++;
	charStack[top]=c;
}

void charPop(void)
{
	while(top>=0&&charStack[top]!='(')
	{
		postCount++;
		postfix[postCount]=charStack[top];
		top--;
		postCount++;
	}
	if(-1==top)
		postCount++;
}

void createPostfixExpression(char *infix)
{
	int inCount=0;		//中缀表达式计数器
	top=-1;
	postCount=0;
	memset(postfix,' ',SIZE);//初始化
	while(infix[inCount]!='\0')
	{
		switch(infix[inCount])
		{
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			postfix[postCount]=infix[inCount];
			postCount++;
			break;
		case '+':case '-':
			postCount++;		//空格
			charPop();
			charPush(infix[inCount]);
			break;
		case '*':case '/':
			postCount++;
			if(charStack[top]=='+'||charStack[top]=='-')
				charPush(infix[inCount]);
			else
				charPop();
			break;
		case '(':
			charPush(infix[inCount]);
			break;
		case ')':
			charPop();
			top--;
			break;
		default:
			printf("illegal input!\n");
			postfix[0]='\0';
			return; 
		}
		inCount++;
	}
	charPop();
	postfix[postCount]='\0';
	printf("%s\n",postfix);
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
	int number=0,right_oprand;		//right_oprand为又操作数
	top=-1;
	postCount=0;		//初始化
	while(postfix[postCount]!='\0')
	{
		switch(postfix[postCount])
		{
		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			number = number*10+(postfix[postCount]-'0');
			break;
		case ' ':
			if(0==postCount)
				break;
			switch(postfix[postCount-1])
			{
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
				case ' ':
					break;
				default :
					intPush(number);
					number=0;
					break;
			}
		case '+':case '-':case '*':case '/':
			break;
		default:
			printf("illegal input\n");
			return -1;
		}
		postCount++;
	}
	
	return intPop();
}
