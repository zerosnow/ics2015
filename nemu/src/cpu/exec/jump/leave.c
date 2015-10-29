#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

#define instr leave

make_helper(leave) {
	cpu.esp = cpu.ebp;
	cpu.ebp = MEM_R(cpu.esp);
	cpu.esp +=4;
	print_asm_template0();
	
	return 1;
}

#include "cpu/exec/template-end.h"

#undef DATA_BYTE

/* for instruction encoding overloading */

