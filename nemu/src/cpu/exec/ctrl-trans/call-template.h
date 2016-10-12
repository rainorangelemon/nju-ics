#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_rel_,SUFFIX)){
	op_src->val=(DATA_TYPE_S)instr_fetch(eip+1,DATA_BYTE);
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
	return DATA_BYTE+1;
}

make_helper(concat(call_rm_,SUFFIX)){
	concat(decode_rm_,SUFFIX)(eip+1);
	if(ops_decoded.is_operand_size_16==true){
		reg_l(R_ESP)=reg_l(R_ESP)-2;
		uint16_t ip=(cpu.eip+DATA_BYTE+1)&(0x0000ffff);
		MEM_W(reg_l(R_ESP),ip);
		cpu.eip=((op_src->val)&(0x0000ffff));		
	}else{
		reg_l(R_ESP)=reg_l(R_ESP)-4;
		MEM_W(reg_l(R_ESP),cpu.eip+DATA_BYTE+1);
		cpu.eip=op_src->val;
	}
	print_asm_template1();
	return 0;
}

#include "cpu/exec/template-end.h"
