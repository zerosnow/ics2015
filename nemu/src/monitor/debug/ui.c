#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <elf.h>

void cpu_exec(uint32_t);
void pretend_cache_read(hwaddr_t , size_t );
uint32_t cache_read(hwaddr_t, size_t);
hwaddr_t page_translate(lnaddr_t);
extern int nr_symtab_entry;
extern Elf32_Sym *symtab;
extern char *strtab;

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
static int cmd_bt(char *args);
static int cmd_cache(char *args);
static int cmd_realcache(char *args);
static int cmd_page(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "single step si for 1 step and si n for n step", cmd_si },
	{ "info", "info r 打印寄存器状态, info w 打印监视点信息, info e 打印标志寄存器信息\ninfo f 打印符号表\n", cmd_info },
	{ "p", "表达式求值, 示例:p $eax+1", cmd_p},
	{ "x", "扫描内存,x N EXPR, 以16进制输出EXPR后N个4字节单元", cmd_x },
	{ "w", "设置监视点,示例w *0x2000,当表达式的值发生变化时停止执行", cmd_w },
	{ "d", "删除监视点,示例d N, 删除监视点序号为N的监视点", cmd_d },
	{ "bt", "打印栈帧链", cmd_bt},
	{ "cache", "cache ADDR 使用ADDR查找cache", cmd_cache},
	{ "realcache", "similar to cache but real read addr", cmd_realcache},
	{ "page", "page ADDR 对ADDR进行页级地址转换", cmd_page}

	/* TODO: Add more commands */
};

typedef struct {
	swaddr_t prev_ebp;
	swaddr_t cur_addr;	//当前函数中的地址
	char cur_funcName[15];	//当前函数名称
	swaddr_t begin_addr;	//当前函数起始地址
	swaddr_t ret_addr;
	uint32_t args[4];
}PartOfStackFrame;

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_si(char *args) {
	int number = 0;
	if(NULL == args) {
		cpu_exec(1);
	}
	else{
		number = strtol(args, NULL, 10);	
		if(number > 0)
			cpu_exec(number);
		else
			printf("parameter error!\n si for 1 step and si n for n step\n");
	}
	return 0;
}

static int cmd_info(char *args) {
	int i;
	if(NULL == args)
		printf("info r 打印寄存器状态, info w 打印监视点信息, info e 打印标志寄存器信息\ninfo f 打印符号表\n");

	else{
		if('r' == args[0]){
			printf("cpu.eax = %x\n", cpu.eax);
			printf("cpu.ebx = %x\n", cpu.ebx);
			printf("cpu.ecx = %x\n", cpu.ecx);
			printf("cpu.edx = %x\n", cpu.edx);
			printf("cpu.esp = %x\n", cpu.esp);
			printf("cpu.ebp = %x\n", cpu.ebp);
			printf("cpu.esi = %x\n", cpu.esi);
			printf("cpu.edi = %x\n", cpu.edi);
			printf("cpu.eip = %x\n", cpu.eip);
		}
		else if('w' == args[0])
			print_wp();
		else  if ('e' == args[0])
		{
			printf("EFLAGS = 0x%08x\n", cpu.eflags);
			printf("CF           = %d\n", cpu.eflags & 0x1);
			printf("PF           = %d\n", (cpu.eflags>>2) & 0x1);
			printf("AF           = %d\n", (cpu.eflags>>4) & 0x1);
			printf("ZF           = %d\n", (cpu.eflags>>6) & 0x1);
			printf("SF           = %d\n", (cpu.eflags>>7) & 0x1);
			printf("IF           = %d\n", (cpu.eflags>>9) & 0x1);
			printf("DF           = %d\n", (cpu.eflags>>10) & 0x1);
			printf("OF           = %d\n", (cpu.eflags>>11) & 0x1);
		}
		else if ('f' == args[0])
		{
			for (i=0; i < nr_symtab_entry; ++i)
				printf("%15s, 0x%08x, %2u, %d\n", (char *)&strtab[symtab[i].st_name], symtab[i].st_value, symtab[i].st_info, symtab[i].st_size);
		}
		else
			printf("info r 打印寄存器状态, info w 打印监视点信息, info e 打印标志寄存器信息\ninfo f 打印符号表\n");
	}
	return 0;
}

static int cmd_p(char *args) {
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
		printf("%x\n",result);
	return 0;
}

static int cmd_x(char *args) {
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

static int cmd_w(char *args) {
	if(NULL==args){
		printf("w EXPR,例如:w $eax+2\n");
		return 0;
	}
	add_wp(args);
	return 0;
}

static int cmd_d(char *args) {
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

static int cmd_bt(char *args) {
	int i;
	uint32_t temp_ebp = cpu.ebp;
	PartOfStackFrame tempStactFrame;
	tempStactFrame.ret_addr = 0;
	if (args != NULL) {
		printf("bt, 打印栈帧链");
		return 0;
	}
	while(temp_ebp != 0) {
		//初始化一些值
		tempStactFrame.cur_funcName[0] = '\0';
		tempStactFrame.begin_addr = 0;
		tempStactFrame.prev_ebp = swaddr_read(temp_ebp, 4);
		tempStactFrame.cur_addr = tempStactFrame.ret_addr?tempStactFrame.ret_addr:cpu.eip;
		for(i=0;i<nr_symtab_entry;i++) {
			if (symtab[i].st_info == 18 && tempStactFrame.cur_addr >= symtab[i].st_value && tempStactFrame.cur_addr <=symtab[i].st_value + symtab[i].st_size) {
				strcpy(tempStactFrame.cur_funcName ,(char *)&strtab[symtab[i].st_name]);
				tempStactFrame.begin_addr = symtab[i].st_value;
			}
		}
		if (temp_ebp+4 == 0x8000000)break;
		tempStactFrame.ret_addr = swaddr_read(temp_ebp+4, 4);
		for (i=0; i<4; ++i) {
			if (temp_ebp+8+4*i == 0x8000000)
				while(i<4)
					tempStactFrame.args[i++] = 0;
			else
				tempStactFrame.args[i] = swaddr_read(temp_ebp+8+4*i, 4);
		}
		temp_ebp = tempStactFrame.prev_ebp;
		printf("0x%08x, %15s(%8d,%8d,%8d,%8d)\n", tempStactFrame.begin_addr, tempStactFrame.cur_funcName,
			tempStactFrame.args[0], tempStactFrame.args[1], tempStactFrame.args[2], tempStactFrame.args[3]);
	}
	return 0;
}

static int cmd_cache(char *args) {
	bool success;
	int addr;
	if (NULL == args) {
		printf("cache ADDR 使用ADDR查找cache\n");
		return 0;
	}
	addr = expr(args,&success);
	if(false==success)
		printf("Expression is wrong\n");
	else
		pretend_cache_read(addr, 4);
	return 0;
}

static int cmd_realcache(char *args) {
	bool success;
	int addr;
	if (NULL == args) {
		printf("cache ADDR 使用ADDR查找cache\n");
		return 0;
	}
	addr = expr(args,&success);
	if(false==success)
		printf("Expression is wrong\n");
	else
		cache_read(addr, 4);
	return 0;
}

static int cmd_page(char *args) {
	bool success;
	int addr;
	if (NULL == args) {
		printf("page ADDR 对ADDR进行页级地址转换\n");
		return 0;
	}
	addr = expr(args,&success);
	if(false==success)
		printf("Expression is wrong\n");
	else
		printf("translate result is %x\n", page_translate(addr));
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
