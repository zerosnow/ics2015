#include "cpu/exec/template-start.h"

#define instr call
static void do_execute() {
	cpu.esp -=4;
	current_sreg.val = cpu.ss.selector;
	MEM_W(cpu.esp, cpu.eip);
	if (MSB(op_src->val)) {
		#if DATA_BYTE == 1
			cpu.eip += 0xffffff00 | op_src->val;
		#elif DATA_BYTE == 2
			cpu.eip += 0xffff0000 | op_src->val;
		#else
			cpu.eip += op_src->val;
		#endif
	}
	else 
		cpu.eip +=op_src->val;
	printf("%x\n", op_src->val);
	call_stack[ptr_call_stack++] = 5;
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
