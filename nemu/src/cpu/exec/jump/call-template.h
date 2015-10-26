#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	cpu.esp -=4;
	MEM_W(cpu.esp, cpu.eip);
	cpu.eip += op_src->addr;
	#if DATA_BYTE == 2
		cpu.eip = cpu.eip & 0x0000ffff;
	#endif
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
