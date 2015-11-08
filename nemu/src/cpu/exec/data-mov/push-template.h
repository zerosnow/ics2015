#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
	#if DATA_BYTE == 2
		cpu.esp -=2;
		MEM_W(cpu.esp, op_src->val);
	#elif DATA_BYTE == 4
		cpu.esp -= 4;
		MEM_W(cpu.esp, op_src->val);
	#endif

	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
