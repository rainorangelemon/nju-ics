#include "cpu/exec/template-start.h"
#define instr bt

static void do_execute(){
	DATA_TYPE left=(DATA_TYPE)op_dest->val;
	DATA_TYPE right=(DATA_TYPE)op_src->val;
	cpu.flags.bits.cf=(left<<(31-right))>>31;
	print_asm_template1();
}

make_instr_helper(i2rm);
make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"
