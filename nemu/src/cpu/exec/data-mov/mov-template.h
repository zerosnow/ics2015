#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}
#if DATA_BYTE == 4
make_helper(mov_cr2r) {
	int len = decode_rm_l(eip+1);
	OPERAND_W(op_src, cpu.cr0.val);
	print_asm("mov %%%s, %%cr0", REG_NAME(op_src->reg));
	return 1+len;
}
make_helper(mov_r2cr) {
	int len = decode_rm_l(eip+1);
	cpu.cr0.val = op_src->val;
	print_asm("mov %%cr0, %%%s", REG_NAME(op_src->reg));
	return 1+len;
}

make_helper(mov_seg) {
	int len = decode_r_l(eip);
	uint8_t opcode = instr_fetch(eip + 1, 1);
	extern SEG_DES *seg_des;
	switch(opcode) {
		case 0xd8:
			cpu.ds.selector = op_src->val;
			Assert(((cpu.ds.selector>>3)<<3) <= cpu.gdtr.seg_limit, "segment out limit %d, %d", ((cpu.ds.selector>>3)<<3), cpu.gdtr.seg_limit);
			seg_des = (SEG_DES *)(cpu.gdtr.base_addr + ((cpu.ds.selector>>3)<<3));
			Assert(seg_des->P == 1, "segment error");
			cpu.ds.seg_base1 = seg_des->seg_base1;
			cpu.ds.seg_base2 = seg_des->seg_base2;
			cpu.ds.seg_limit1 = seg_des->seg_limit1;
			cpu.ds.seg_limit2 = seg_des->seg_limit2;
			print_asm("mov %%%s, ds", REG_NAME(op_src->reg));
			break;
		case 0xc0:
			cpu.es.selector = op_src->val;
			Assert(((cpu.es.selector>>3)<<3) <= cpu.gdtr.seg_limit, "segment out limit %d, %d", ((cpu.es.selector>>3)<<3), cpu.gdtr.seg_limit);
			seg_des = (SEG_DES *)(cpu.gdtr.base_addr + ((cpu.es.selector>>3)<<3));
			Assert(seg_des->P == 1, "segment error");
			cpu.es.seg_base1 = seg_des->seg_base1;
			cpu.es.seg_base2 = seg_des->seg_base2;
			cpu.es.seg_limit1 = seg_des->seg_limit1;
			cpu.es.seg_limit2 = seg_des->seg_limit2;
			print_asm("mov %%%s, es", REG_NAME(op_src->reg));
			break;
		break;
		case 0xd0:
			cpu.ss.selector = op_src->val;
			Assert(((cpu.ss.selector>>3)<<3) <= cpu.gdtr.seg_limit, "segment out limit %d, %d", ((cpu.ss.selector>>3)<<3), cpu.gdtr.seg_limit);
			seg_des = (SEG_DES *)(cpu.gdtr.base_addr + ((cpu.ss.selector>>3)<<3));
			Assert(seg_des->P == 1, "segment error");
			cpu.ss.seg_base1 = seg_des->seg_base1;
			cpu.ss.seg_base2 = seg_des->seg_base2;
			cpu.ss.seg_limit1 = seg_des->seg_limit1;
			cpu.ss.seg_limit2 = seg_des->seg_limit2;
			print_asm("mov %%%s, ss", REG_NAME(op_src->reg));
			break;
		break;
		default:
		break;
	}
	return 1+len;
}
#endif 

#include "cpu/exec/template-end.h"
