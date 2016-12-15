#include "cpu/exec/template-start.h"

void pio_write(ioaddr_t addr,size_t len, uint32_t data);
/*because there is no a2i, so I use make_helper*/
make_helper(concat(out_d_,SUFFIX)){
	uint16_t index=reg_w(R_DX);
	pio_write(index,DATA_BYTE,(DATA_TYPE)REG(R_EAX));
	print_asm("out_d");
	return 1;
}


make_helper(concat(out_i_,SUFFIX)){
	uint16_t index=instr_fetch(eip+1,1);
	pio_write(index,DATA_BYTE,(DATA_TYPE)REG(R_EAX));
	print_asm("out_i");
	return 2;
}

#include "cpu/exec/template-end.h"
