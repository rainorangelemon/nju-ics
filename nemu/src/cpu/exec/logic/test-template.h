#include "cpu/exec/template-start.h"

#define instr test

static void do_execute (){
	DATA_TYPE result = (op_src->val)&(op_dest->val);
	set_sf(result,DATA_BYTE);
	set_zf(result);
	set_pf(result);
	cpu.flags.bits.of=0;
	cpu.flags.bits.cf=0;
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
