#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute(){
	DATA_TYPE src_index=concat(reg_,SUFFIX)(R_EAX);
	uint32_t dest_index=reg_l(R_EDI);
	MEM_W(dest_index,src_index,R_ES);
	if(cpu.flags.bits.df==0){
		reg_l(R_EDI)=reg_l(R_EDI)+DATA_BYTE;
	}else{
		reg_l(R_EDI)=reg_l(R_EDI)-DATA_BYTE;	
	}
	print_asm_template1();
}

make_helper(concat3(instr,_,SUFFIX)){
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"
