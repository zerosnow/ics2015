#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	DATA_TYPE result = op_dest->val - (DATA_TYPE)op_src->val;
	OPERAND_W(op_dest, result);
	
	print_asm_template1();
}

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
