#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	unsigned int result=MEM_R(reg_l(R_ESP));
	OPERAND_W(op_src,result);
	if(ops_decoded.is_operand_size_16==true)
		reg_l(R_ESP)=reg_l(R_ESP)+2;
	else
		reg_l(R_ESP)=reg_l(R_ESP)+4;
	print_asm_template1();	
}

#if((DATA_BYTE==2)||(DATA_BYTE==4))
make_instr_helper(rm)
make_instr_helper(r)
#endif


#include "cpu/exec/template-end.h"
