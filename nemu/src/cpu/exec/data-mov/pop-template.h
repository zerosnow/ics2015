#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute () {
	current_sreg.val = cpu.ss.selector;
	OPERAND_W(op_src ,MEM_R(cpu.esp));
	cpu.esp += DATA_BYTE;

	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
