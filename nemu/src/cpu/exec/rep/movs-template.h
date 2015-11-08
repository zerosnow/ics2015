#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat3(instr, _, SUFFIX)) {
	swaddr_write(cpu.edi,  DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE));
	cpu.edi += DATA_BYTE;
	cpu.esi += DATA_BYTE;
	return 1;
}

#include "cpu/exec/template-end.h"
