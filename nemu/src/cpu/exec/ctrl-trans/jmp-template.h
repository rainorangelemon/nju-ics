#include "cpu/exec/template-start.h"

#define instr jmp

make_helper(concat(jmp_rel_,SUFFIX)){
	op_src->val=(DATA_TYPE_S)instr_fetch(eip+1,DATA_BYTE);
	cpu.eip=cpu.eip+op_src->val;
	if(ops_decoded.is_operand_size_16==true){
		cpu.eip=cpu.eip&0xffff;
	}
	print_asm_template1();
	return DATA_BYTE+1;
}

#if ((DATA_BYTE==2) || (DATA_BYTE==4))
make_helper(concat(jmp_rm_,SUFFIX)){
	concat(decode_rm_,SUFFIX)(eip+1);
	if(ops_decoded.is_operand_size_16==true)
		cpu.eip=(op_src->val)&0xffff;
	else
		cpu.eip=op_src->val;
	print_asm_template1();
	return 0;
}
#endif

#include "cpu/exec/template-end.h"
