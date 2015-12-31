#include "cpu/stack.h"
extern int ptr_call_stack;

#define instr ret

#define DATA_BYTE 2
#include "cpu/exec/template-start.h"
make_helper(ret_w) {
	current_sreg.val = cpu.ss.selector;
	cpu.eip = (cpu.eip & 0xffff0000) | MEM_R(cpu.esp);
	cpu.esp +=2;
	print_asm_template0();
	if (ptr_call_stack <= 0)
		return 5;
	return call_stack[--ptr_call_stack];
}
#include "cpu/exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cpu/exec/template-start.h"
make_helper(ret_l) {
	current_sreg.val = cpu.ss.selector;
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

