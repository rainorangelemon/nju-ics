#include "cpu/exec/helper.h"

make_helper(lidt){
	int len=decode_rm_b(eip+1);
	if(ops_decoded.is_operand_size_16){
		cpu.idtr.limit=lnaddr_read(op_src->addr,2);
		cpu.idtr.base=lnaddr_read(op_src->addr+2,3);
	}else{
		cpu.idtr.limit=lnaddr_read(op_src->addr,2);
		cpu.idtr.base=lnaddr_read(op_src->addr+2,4);
	}
	print_asm("lidt");
	return len+1;
}
