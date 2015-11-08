#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	cpu.OF = MSB(result) & (~(MSB(op_src->val)));
	cpu.SF = MSB(result);
	cpu.ZF = (result == 0); 
	cpu.AF =((op_src->val>>3)&0x1) < ((result>>3) &0x1); 
	cpu.PF = ~( (result & 0x1) & ((result>>1) & 0x1) & ((result>>2) & 0x1) & ((result>>3) & 0x1) & 
	((result>>4) & 0x1) & ((result>>5) & 0x1) & ((result>>6) & 0x1) & ((result>>7) & 0x1));
	OPERAND_W(op_src, result);

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
