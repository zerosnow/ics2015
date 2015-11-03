#define DATA_BYTE 4
#include "cpu/stack.h"
#include "cpu/exec/template-start.h"
extern int ptr_call_stack;


#define instr ret

make_helper(ret) {
	cpu.eip = MEM_R(cpu.esp);
	cpu.esp +=4;
	print_asm_template0();
	if (ptr_call_stack <= 0)
		return 5;
	return call_stack[--ptr_call_stack];
}

#include "cpu/exec/template-end.h"

#undef DATA_BYTE

/* for instruction encoding overloading */

