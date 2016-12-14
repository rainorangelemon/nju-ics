#include "cpu/exec/helper.h"
void raise_intr(uint8_t NO);

make_helper(int_ib){
	uint8_t index=(uint8_t)instr_fetch(eip+1,1);
	cpu.eip=cpu.eip+2;
	raise_intr(index);
	return 0;
}
