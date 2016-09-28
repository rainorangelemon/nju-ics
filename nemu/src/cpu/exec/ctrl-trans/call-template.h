#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	reg_l(R_ESP)=reg_l(R_ESP)-2;
	MEM_W(reg_l(R_ESP),cpu.eip+DATA_BYTE+1);
	cpu.eip=cup.eip+op_src->imm;
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
