#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	if (op_src->type == OP_TYPE_SIMM && (op_src->simm >> 7) == 1) {
		op_src->simm = 0xffffff00 | op_src->simm;
		op_src->val = op_src->simm;
	}
	DATA_TYPE result = op_dest->val - op_src->val;
	cpu.OF = ((~(MSB(op_dest->val)))&(MSB(op_src->val))&(MSB(result))) || 
		((MSB(op_dest->val))&(~(MSB(op_src->val)))&(~(MSB(result))));
	cpu.SF = MSB(result);
	cpu.ZF = (result == 0);
	cpu.AF =((op_dest->val>>3)&0x1) < ((op_src->val>>3)&0x1) + ((result>>3) &0x1);
	cpu.PF = ~( (result & 0x1) & ((result>>1) & 0x1) & ((result>>2) & 0x1) & ((result>>3) & 0x1) & 
	((result>>4) & 0x1) & ((result>>5) & 0x1) & ((result>>6) & 0x1) & ((result>>7) & 0x1));
	cpu.CF = MSB(op_dest->val) < MSB(op_src->val) + MSB(result);
	OPERAND_W(op_dest, result);

	print_asm_template2();
}


#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(i2a)


#include "cpu/exec/template-end.h"
