#include "nemu.h"


lnaddr_t seg_translate(swaddr_t addr, size_t len, SELECTOR current_sreg) {
	assert(addr + len < cpu.gdtr.seg_limit);
	return addr;
}