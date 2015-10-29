#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

#define instr ret

make_helper(ret) {
	cpu.esp +=4;
	cpu.eip = MEM_R(cpu.esp);
	print_asm_template1();
	
	return 1;
}

#include "cpu/exec/template-end.h"

#undef DATA_BYTE

/* for instruction encoding overloading */

