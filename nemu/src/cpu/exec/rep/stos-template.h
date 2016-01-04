#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat3(instr, _, SUFFIX)) {
	#if DATA_BYTE == 1
		swaddr_write(cpu.edi,  DATA_BYTE, reg_b(R_AL));
	#elif DATA_BYTE == 2
		swaddr_write(cpu.edi,  DATA_BYTE, reg_w(R_AX));
	#elif DATA_BYTE == 4
		swaddr_write(cpu.edi,  DATA_BYTE, reg_l(R_EAX));
	#endif

	if (cpu.DF == 0) 
		cpu.edi += DATA_BYTE;
	else
		cpu.edi -= DATA_BYTE;
	return 1;
}

#include "cpu/exec/template-end.h"
