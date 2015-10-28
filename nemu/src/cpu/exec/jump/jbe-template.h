#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute() {
	if (cpu.CF == 1 || cpu.ZF == 1)
	{
		cpu.eip += op_src->addr;
		#if DATA_BYTE == 2
			cpu.eip &= 0x0000ffff;
		#endif
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
