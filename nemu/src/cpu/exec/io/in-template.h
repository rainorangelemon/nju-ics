#include "cpu/exec/template-start.h"

uint32_t pio_read(ioaddr_t addr,size_t len);
make_helper(concat(in_d_,SUFFIX)){
	uint16_t index=reg_w(R_DX);
	REG(R_EAX)=(uint32_t)(DATA_TYPE)pio_read(index,DATA_BYTE);
	print_asm("in_d");
	return 1;
}


make_helper(concat(in_i_,SUFFIX)){
	uint16_t index=instr_fetch(eip+1,1);
	REG(R_EAX)=(uint32_t)(DATA_TYPE)pio_read(index,DATA_BYTE);
	print_asm("in_i");
	return 2;
}

#include "cpu/exec/template-end.h"
