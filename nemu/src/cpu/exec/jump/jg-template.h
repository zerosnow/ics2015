#include "cpu/exec/template-start.h"

#define instr jg

static void do_execute() {
	if (cpu.ZF == 0 && cpu.SF == cpu.OF)
	{
		if (MSB(op_src->val)) {
			#if DATA_BYTE == 1
				cpu.eip += 0xffffff00 | op_src->val;
			#elif DATA_BYTE == 2
				cpu.eip += 0xffff0000 | op_src->val;
			#else
				cpu.eip += op_src->val;
			#endif
		}
		else 
			cpu.eip +=op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
