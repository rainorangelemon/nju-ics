#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	reg_l(R_ESP)=reg_l(R_ESP)-2;
	MEM_W(reg_l(R_ESP),reg_l(op_src->reg));	
	print_asm_template1();
}

make_instr_helper(r);

#include "cpu/exec/template-end.h"
