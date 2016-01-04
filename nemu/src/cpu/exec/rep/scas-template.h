#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat3(instr, _, SUFFIX)) {
	#if DATA_BYTE == 1
		if (swaddr_read(cpu.edi,  DATA_BYTE) == reg_b(R_AL)) 
			return 0;
	#elif DATA_BYTE == 2
		if (swaddr_read(cpu.edi,  DATA_BYTE) == reg_w(R_AX)) 
			return 0;
	#elif DATA_BYTE == 4
		if (swaddr_read(cpu.edi,  DATA_BYTE) == reg_l(R_EAX)) 
			return 0;
	#endif
	if (cpu.DF == 0) 
		cpu.edi += DATA_BYTE;
	else
		cpu.edi -= DATA_BYTE;
	return 1;
}

#include "cpu/exec/template-end.h"
