#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	if(ops_decoded.is_operand_size_16==true){
		reg_l(R_ESP)=reg_l(R_ESP)-2;
		MEM_W(reg_l(R_ESP),op_src->val);
	}else{
		reg_l(R_ESP)=reg_l(R_ESP)-4;
		MEM_W(reg_l(R_ESP),op_src->val);
	}	
	print_asm_template1();
}

make_instr_helper(i)
#if ((DATA_BYTE ==2) || (DATA_BYTE == 4))
make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
