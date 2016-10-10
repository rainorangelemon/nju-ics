#include "cpu/exec/template-start.h"

#define instr lods

static void do_execute(){
	int IncDec=0;
	uint32_t src_index=reg_l(R_ESI);
	MEM_W(REG(R_EAX),MEM_R(src_index));
	if(cpu.flags.bits.df==0){
		IncDec=DATA_BYTE;
	}else{
		IncDec=-DATA_BYTE;	
	}
	reg_l(R_ESI)=reg_l(R_ESI)+IncDec;
	print_asm(str(instr));
}

make_helper(concat3(instr,_,SUFFIX)){
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"
