#include "cpu/exec/helper.h"

make_helper(lgdt){
	int len=decode_rm_b(eip+1);
	if(ops_decoded.is_operand_size_16){
		cpu.gdtr.limit=lnaddr_read(op_src->addr,2);
		cpu.gdtr.base=lnaddr_read(op_src->addr+2,3);
	}else{
		cpu.gdtr.limit=lnaddr_read(op_src->addr,2);
		cpu.gdtr.base=lnaddr_read(op_src->addr+2,4);
	}
	print_asm("lgdt");
	return len+1;
}
