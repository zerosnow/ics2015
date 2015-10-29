#define DATA_BYTE 4
#include "cpu/exec/template-start.h"


make_helper(leave) {
	cpu.esp = cpu.ebp;
	cpu.esp +=4;
	cpu.ebp = MEM_R(cpu.esp);
	print_asm_template1();
	
	return 1;
}

#include "cpu/exec/template-end.h"

#undef DATA_BYTE

/* for instruction encoding overloading */

