#include "cpu/exec/template-start.h"

#define instr call2


static void do_execute() {
	cpu.esp -=4;
	current_sreg.val = cpu.ss.selector;
	MEM_W(cpu.esp, cpu.eip);
	#if DATABYTE == 2 
		cpu.eip = op_src->val & 0x0000ffff - 2;
	#else
		cpu.eip = op_src->val - 2 ;
	#endif
	call_stack[ptr_call_stack++] = 2;
	print_asm_template1();
}


make_instr_helper(rm)

#include "cpu/exec/template-end.h"
