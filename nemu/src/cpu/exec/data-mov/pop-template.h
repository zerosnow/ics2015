#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute () {
	#if DATA_BYTE == 2
		OPERAND_W(op_src ,MEM_R(cpu.esp));
		cpu.esp +=2;
	#elif DATA_BYTE == 4
		OPERAND_W(op_src ,MEM_R(cpu.esp));
		cpu.esp += 4;
	#endif

	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
