#include "cpu/exec/template-start.h"

#define instr jmp



static void do_execute() {
	if (MSB(op_src->val)) {
		#if DATA_BYTE == 1
			cpu.eip += 0xffffff00 | op_src->val;
		#elif DATA_BYTE == 2
			cpu.eip += 0xffff0000 | op_src->val;
		#else
			cpu.eip += op_src->val;
		#endif
	}
	else 
		cpu.eip +=op_src->val;
	print_asm_template1();
}

#if DATA_BYTE == 4

make_helper(ljmp) {
	extern SEG_DES *seg_des;
	SEG_DES seg;
	seg_des = &seg;
	uint32_t op1 = instr_fetch(eip+1, 4)-7;
	uint16_t op2 = instr_fetch(eip +5, 2);
	cpu.eip = op1;
	cpu.cs.selector = op2;
	Assert(((cpu.cs.selector>>3)<<3) <= cpu.gdtr.seg_limit, "segment out limit %d, %d", ((cpu.cs.selector>>3)<<3), cpu.gdtr.seg_limit);
	seg_des->val_part1 = instr_fetch(cpu.gdtr.base_addr + ((cpu.cs.selector>>3)<<3), 4);
	seg_des->val_part2 = instr_fetch(cpu.gdtr.base_addr + ((cpu.cs.selector>>3)<<3)+4, 4);
	Assert(seg_des->P == 1, "segment error");
	cpu.cs.seg_base1 = seg_des->seg_base1;
	cpu.cs.seg_base2 = seg_des->seg_base2;
	cpu.cs.seg_base3 = seg_des->seg_base3;
	cpu.cs.seg_limit1 = seg_des->seg_limit1;
	cpu.cs.seg_limit2 = seg_des->seg_limit2;
	cpu.cs.seg_limit3 = 0xfff;
	print_asm("ljmp %x, %x\n", op2, op1+7);
	return 7;
}



#endif

make_instr_helper(i)

#include "cpu/exec/template-end.h"
