#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	if(ops_decoded.is_operand_size_16==true){
		reg_l(R_ESP)=reg_l(R_ESP)-2;
		uint16_t ip=(cpu.eip+DATA_BYTE+1)&(0x0000ffff);
		MEM_W(reg_l(R_ESP),ip);
		cpu.eip=((cpu.eip+op_src->val+DATA_BYTE+1)&(0x0000ffff))-DATA_BYTE-1;		
	}else{
		reg_l(R_ESP)=reg_l(R_ESP)-4;
		MEM_W(reg_l(R_ESP),cpu.eip+DATA_BYTE+1);
		cpu.eip=cpu.eip+op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
