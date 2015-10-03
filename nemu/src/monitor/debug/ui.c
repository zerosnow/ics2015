#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}
static int cmd_q(char *args) {
	return -1;
}
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_help(char *args);
static int cmd_p(char *args);
static int cmd_x(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "single step si for 1 step and si n for n step", cmd_si },
	{ "info", "info r 打印寄存器状态, info w 打印监视点信息", cmd_info },
	{ "p", "表达式求值, 示例:p $eax+1", cmd_p},
	{ "x", "扫描内存,x N EXPR, 以16进制输出EXPR后N个4字节单元", cmd_x },
	{ "w", "设置监视点,示例w *0x2000,当表达式的值发生变化时停止执行", cmd_w },
	{ "d", "删除监视点,示例d N, 删除监视点序号为N的监视点", cmd_d }
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_si(char *args){
	int number = 0;
	if(NULL == args)
		cpu_exec(1);
	else{
		number = strtol(args, NULL, 10);	
		if(number > 0)
			cpu_exec(number);
		else
			printf("parameter error!\n si for 1 step and si n for n step\n");
	}
	return 0;
}

static int cmd_info(char *args){
	if(NULL == args)
		printf("info r 打印寄存器状态, info w 打印监视点信息\n");
	else{
		if('r' == args[0]){
			printf("cpu.eax = %d\n", cpu.eax);
			printf("cpu.ebx = %d\n", cpu.ebx);
			printf("cpu.ecx = %d\n", cpu.ecx);
			printf("cpu.edx = %d\n", cpu.edx);
			printf("cpu.esp = %d\n", cpu.esp);
			printf("cpu.ebp = %d\n", cpu.ebp);
			printf("cpu.esi = %d\n", cpu.esi);
			printf("cpu.edi = %d\n", cpu.edi);
			printf("cpu.eip = %d\n", cpu.eip);
		}
		else if('w' == args[0])
			print_wp();
		else 
			printf("info r 打印寄存器状态, info w 打印监视点信息\n");
	}
	return 0;
}

static int cmd_p(char *args){
	bool success;
	int result;
	if(NULL==args){
		printf("p EXPR,例如:p 2+4\n");
		return 0;
	}
	result = expr(args,&success);
	if(false==success)
		printf("Expression is wrong\n");
	else
		printf("%d\n",result);
	return 0;
}

static int cmd_x(char *args){
	char *csize = strtok(args, " ");
	char *caddr = csize+strlen(csize)+1;
	int size = atoi(csize);
	int addr;
	int i;
	bool success;
	if(NULL == csize||NULL == caddr)
		printf("x N EXPR,例如:x 10 $eax\n");
	else{
		printf("%s\n",caddr);
		addr = expr(caddr,&success);
		addr = addr?((addr-1)/4+1)*4:0;
		if(false==success)
			printf("Expression is wrong\n");
		else
			for(i=0;i<size;i++)
				printf("%d\n",swaddr_read(addr+i*4,4));
	}
	return 0;
}

static int cmd_w(char *args){
	if(NULL==args){
		printf("w EXPR,例如:w $eax+2\n");
		return 0;
	}
	add_wp(args);
	return 0;
}

static int cmd_d(char *args){
	int n;
	if(NULL==args){
		printf("d N,例如：d 2\n");
		return 0;
	}
	n = atoi(args);
	if(true==delete_wp(n))
		printf("delete successful\n");
	else
		printf("delete failed,no this watchpoint or args error\n");
	return 0;
}

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
