#include "cpu/exec/template-start.h"

#define instr jmp2

static void do_execute() {
	#if DATABYTE == 2 
		cpu.eip = op_src->val & 0x0000ffff;
	#elif DATABYTE == 4
		cpu.eip = op_src->val;
	#endif
	print_asm_template1();
}


make_instr_helper(i)

#include "cpu/exec/template-end.h"
