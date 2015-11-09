#include "cpu/exec/template-start.h"

#define instr push

static void do_execute () {
	cpu.esp -= DATA_BYTE;
	MEM_W(cpu.esp, op_src->val);
	printf("1111111111111111\n");
	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
