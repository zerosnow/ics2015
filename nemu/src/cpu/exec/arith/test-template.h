#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	

	/* TODO: Update EFLAGS. */
	//panic("please implement me");

	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)


#include "cpu/exec/template-end.h"
