#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	if (op_src->type == OP_TYPE_SIMM && (op_src->simm >> 7) == 1) {
		op_src->simm = 0xffffff00 | op_src->simm;
		op_src->val = op_src->simm;
	}
	DATA_TYPE result = op_dest->val | op_src->val;
	OPERAND_W(op_dest, result);
	cpu.CF = 0;
	cpu.OF = 0;
	cpu.PF = ~( (result & 0x1) & ((result>>1) & 0x1) & ((result>>2) & 0x1) & ((result>>3) & 0x1) & 
	((result>>4) & 0x1) & ((result>>5) & 0x1) & ((result>>6) & 0x1) & ((result>>7) & 0x1));
	cpu.SF = MSB(result);
	cpu.ZF = (result == 0); 

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
