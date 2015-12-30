#include "nemu.h"

typedef struct {
	uint64_t seg_limit1	:16;
	uint64_t seg_base1	:24;
	uint64_t A 		:1;
	uint64_t TYPE		:3;
	uint64_t one 		:1;
	uint64_t DPL		:2;
	uint64_t P		:1;
	uint64_t seg_limit2	:4;
	uint64_t AVL		:1;
	uint64_t zero		:1;
	uint64_t X		:1;
	uint64_t G 		:1;
	uint64_t seg_base2	:8;
}SEG_DES;

typedef union {
	struct {
		uint32_t seg_base1 	:24;
		uint32_t seg_base2 	:8;
	};
	uint32_t val;
}SEG_BASE;

typedef union {
	struct {
		uint32_t seg_limit1 	:16;
		uint32_t seg_limit2 	:4;
	};
	uint32_t val;
}SEG_LIMIT;


lnaddr_t seg_translate(swaddr_t addr, size_t len, SELECTOR current_sreg) {
	Assert((current_sreg.index<<3) <= cpu.gdtr.seg_limit, "segment out limit %d, %d", current_sreg.index<<3, cpu.gdtr.seg_limit);
	SEG_DES *seg_des = (SEG_DES *)(cpu.gdtr.base_addr + (current_sreg.index<<3));
	SEG_BASE seg_base;
	SEG_LIMIT seg_limit;
	if (cpu.cr0.protect_enable == 0 || current_sreg.val == cpu.cs)return addr;		//not protect model
	Assert(seg_des->P == 1, "segment error");
	seg_base.seg_base1 = seg_des->seg_base1;
	seg_base.seg_base2 =  seg_des->seg_base2;
	seg_limit.seg_limit1 = seg_des->seg_limit1;
	seg_limit.seg_limit2 = seg_des->seg_limit2;
	Assert(addr+len < seg_limit.val, "segment out limit");

	return seg_base.val+ addr;
}