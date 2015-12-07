#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	DATA_TYPE result = op_src->val & op_dest->val;
	cpu.CF = 0;
	cpu.PF = ~( (result & 0x1) & ((result>>1) & 0x1) & ((result>>2) & 0x1) & ((result>>3) & 0x1) & 
	((result>>4) & 0x1) & ((result>>5) & 0x1) & ((result>>6) & 0x1) & ((result>>7) & 0x1));
	cpu.ZF =  (result == 0);
	cpu.SF = MSB(result);
	cpu.OF = 0;
	//printf("%d, %d, %d\n", op_src->type, op_dest->type, OP_TYPE_REG);
	// if ((op_src->type == OP_TYPE_REG) && (op_dest->type == OP_TYPE_REG) && op_src->reg != R_CL)
	// 	cpu.eip -= 1;

	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)


#include "cpu/exec/template-end.h"
