#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
 #include <elf.h>

extern int nr_symtab_entry;
extern Elf32_Sym *symtab;
extern char *strtab;

static struct rule {
	char *regex;
	int token_type;
	int priority;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{" +",	NOTYPE, 0},				// spaces
	{"==", EQU, 7},
	{"\\!=", UNEQU, 7},
	{"\\&\\&", AND, 11},
	{"\\|\\|", OR, 12},
	{"\\!", NOT, 2},
	{"\\+", '+', 4},					// plus
	{"\\-", '-', 4},
	{"\\*", '*', 3},
	{"\\/", '/', 3},
	{"^0x[0-9a-fA-F]+",HEX, 0},
	{"^[0-9]+",NUM, 0},					//number
	{"^\\$[a-z]+",REG, 0},					//register
	{"\\(", '(', 15},	
	{"\\)", ')', 15},
	{"^[A-Za-z_][A-Za-z0-9_]*", SYMBOL, 0}
	
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

static int nr_token;
Token tokens[32];

void init_token(){		//将token的type初始化为0方便之后的判断
	int i;
	for(i=0;i<32;++i)
		tokens[i].type=0;
}

int hexToi(char *hex)
{
	int number=0;
	int i=0;
	if(hex[0]=='0'&&hex[1]=='x')
		hex=hex+2;
	while(hex[i]!='\0')
	{
		if(hex[i]>='0'&&hex[i]<='9')
			number=number*16+hex[i]-'0';
		else if(hex[i]>='a'&&hex[i]<='f')
			number=number*16+hex[i]-'a'+10;
		else if(hex[i]>='A'&&hex[i]<='F')
			number=number*16+hex[i]-'A'+10;
		else
			return -1;
		i++;
	}
	return number;
}
	

static bool make_token(char *e) {
	int position = 0;
	int i;
	int j=0;	
	char *reg_name[] = {"eax","ecx","edx","ebx","esp","ebp","esi","edi"};
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				
				// Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				switch(rules[i].token_type) {
					case NOTYPE:
						break;
					case REG:
						strncpy(tokens[nr_token].str, substr_start+1, substr_len-1);
						tokens[nr_token].str[substr_len-1]='\0';
						for(j=0;j<8;j++)
						{
							if(0 == strcmp(tokens[nr_token].str,reg_name[j]))
							{
								sprintf(tokens[nr_token].str, "%d", cpu.gpr[j]._32);
								tokens[nr_token].type = NUM;
								break;
							}

						}
						if(0 == strcmp(tokens[nr_token].str,"eip"))
						{
							sprintf(tokens[nr_token].str, "%d", cpu.eip);
							tokens[nr_token].type = NUM;
						}
						else if(8 == j)
						{
							panic("register is error");
							return false;
						}
						tokens[nr_token].priority=rules[i].priority;
						nr_token++;
						break;
					case '*':
						if(0==nr_token||(tokens[nr_token-1].type!=')'&&tokens[nr_token-1].type!=NUM))
						{
							tokens[nr_token].type=ADDR;
							tokens[nr_token].priority=2;
						}
						else
						{
							tokens[nr_token].type=rules[i].token_type;
							tokens[nr_token].priority=rules[i].priority;
						}
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len]='\0';
						nr_token++;
						break;
					case '-':
						if(0==nr_token||(tokens[nr_token-1].type!=')'&&tokens[nr_token-1].type!=NUM))
						{
							tokens[nr_token].type=MINUS;
							tokens[nr_token].priority=2;
						}
						else
						{
							tokens[nr_token].type=rules[i].token_type;
							tokens[nr_token].priority=rules[i].priority;
						}
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len]='\0';
						nr_token++;
						break;

					case HEX:
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len]='\0';
						sprintf(tokens[nr_token].str, "%d", hexToi(tokens[nr_token].str));
						tokens[nr_token].priority=rules[i].priority;
						tokens[nr_token].type=NUM;
						nr_token++;
						break;
					case SYMBOL:
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len]='\0';
						for(i=0;i<nr_symtab_entry;i++) {
							if (symtab[i].st_info == 17 && strcmp((char *)&strtab[symtab[i].st_name], tokens[nr_token].str) == 0)
							{
								sprintf(tokens[nr_token].str, "%u", symtab[i].st_value);
								tokens[nr_token].type = NUM;
								nr_token++;
								break;
							}
						}
						break;
						
					default: 
						tokens[nr_token].priority=rules[i].priority;
						tokens[nr_token].type=rules[i].token_type;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len]='\0';
						nr_token++;
						//panic("please implement me");
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}
/*
void scanToken(){
	int i;
	for(i=0;i<nr_token;i++){
		printf("%d,%s,%d\n",postfix[i].type,postfix[i].str,postfix[i].priority);
	}
}*/

uint32_t expr(char *e, bool *success) {
	//int i=0;
	init_token();
	*success = true;
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	createPostfixExpression(tokens);
/*	while(postfix[i].type!=0){
		printf("%d,%s,%d\n",postfix[i].type,postfix[i].str,postfix[i].priority);
		i++;
	}*/
	return calPostfixExpression();
}

