#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

#define instr ret

make_helper(ret) {
	cpu.eip = MEM_R(cpu.esp);
	printf("111111111111%x\n", cpu.eip);
	cpu.esp +=4;
	print_asm_template1();
	
	return 5;
}

#include "cpu/exec/template-end.h"

#undef DATA_BYTE

/* for instruction encoding overloading */

