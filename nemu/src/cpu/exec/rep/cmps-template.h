#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat3(instr, _, SUFFIX)) {
	if (swaddr_read(cpu.edi,  DATA_BYTE) == swaddr_read(cpu.esi, DATA_BYTE)) {
		if (cpu.DF == 0) {
			cpu.edi += DATA_BYTE;
			cpu.esi += DATA_BYTE;
		}else {
			cpu.edi -= DATA_BYTE;
			cpu.esi -= DATA_BYTE;
		}
		return 1;
	}
	else return 0;
}

#include "cpu/exec/template-end.h"
