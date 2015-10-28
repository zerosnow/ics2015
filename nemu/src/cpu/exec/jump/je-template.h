#include "cpu/exec/template-start.h"

#define instr je

static void do_execute() {
	if (cpu.ZF == 1)
	{
		#if DATA_BYTE == 1
			cpu.eip = (cpu.eip & 0xffffff00)| (((cpu.eip & 0xff) + (op_src->addr & 0xff)) & 0xff);
		#elif DATA_BYTE == 2
			cpu.eip = (cpu.eip & 0xffff0000)| (((cpu.eip & 0xffff) + (op_src->addr & 0xffff)) & 0xffff);
		#else
			cpu.eip += op_src->addr;
		#endif
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
