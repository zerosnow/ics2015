#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute () {
	printf("%d, %d\n", DATA_BYTE, op_src->val);
	#if DATA_BYTE == 1
		op_src->val = 0xffffff00 | op_src->val;
	#elif DATA_BYTE == 2
		op_src->val = 0xffff0000 | op_src->val;
	#endif
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
