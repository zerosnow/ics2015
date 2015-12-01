#include "cpu/exec/template-start.h"

#define instr jmp2

static void do_execute() {
	#if DATABYTE == 2 
		cpu.eip = op_src->val & 0x0000ffff - 2;
	#else
		if (op_src->type == OP_TYPE_MEM) 
			cpu.eip = op_src->val - 1;
		else
			cpu.eip = op_src->val - 2;
	#endif
	print_asm_template1();
}


make_instr_helper(rm)

#include "cpu/exec/template-end.h"
