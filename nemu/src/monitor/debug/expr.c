#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
	NOTYPE = 256, EQ, NUM, REG,

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"==", EQ},						// equal
	{"^[0-9]+",NUM},					//number
	{"^\\$[a-z]+",REG},					//register
	{"\\(", '('},	
	{"\\)", ')'}
	
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

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

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
				
				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case REG:
						strncpy(tokens[nr_token].str, substr_start+1, substr_len-1);
						for(j=0;j<8;j++)
						{
							
							printf("%d:%s,%s\n",j,tokens[nr_token].str,reg_name[j]);
							if(0 == strcmp(tokens[nr_token].str,reg_name[j]))
							{
								printf("%d\n",j);
								sprintf(tokens[nr_token].str, "%d", cpu.gpr[i]._32);
								tokens[nr_token].type = NUM;
								break;
							}

						}
						printf("%d\n",j);
						if(0 == strcmp(tokens[nr_token].str,"eip"))
						{
							sprintf(tokens[nr_token].str, "%d", cpu.gpr[i]._32);
							tokens[nr_token].type = NUM;
						}
						else if(8 == j)
						{
							panic("register is error");
							return false;
						}
						nr_token++;
						break;
					default: 
						tokens[nr_token].type=rules[i].token_type;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
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

uint32_t expr(char *e, bool *success) {
	int i;
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	for(i=0;i<nr_token;i++)
		printf("%d : %s\n",tokens[i].type,tokens[i].str);

	/* TODO: Insert codes to evaluate the expression. */
//	panic("please implement me");
	return 0;
}

