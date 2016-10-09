#include "cpu/exec/template-start.h"

#define instr jmp

make_helper(concat(jmp_rel_,SUFFIX)){
	int len=concat(decode_si_,SUFFIX)(eip+1);
	cpu.eip=cpu.eip+op_src->val;
	if(ops_decoded.is_operand_size_16==true){
		cpu.eip=cpu.eip&0xffff;
	}
	return len+1;
}

#if ((DATA_BYTE==2) || (DATA_BYTE==4))
make_helper(concat(jmp_rm_,SUFFIX)){
	concat(decode_rm_,SUFFIX)(eip+1);
	if(ops_decoded.is_operand_size_16==true)
		cpu.eip=(op_src->val)&0xffff;
	else
		cpu.eip=op_src->val;
	return 0;
}
#endif

#include "cpu/exec/template-end.h"
