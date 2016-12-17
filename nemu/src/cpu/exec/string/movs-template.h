#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute(){
	int IncDec=0;
	uint32_t src_index=reg_l(R_ESI);
	uint32_t dest_index=reg_l(R_EDI);
	MEM_W(dest_index,MEM_R(src_index,R_DS),R_ES);
	if(reg_l(R_ESI)==0xc01030c6){
		printf("the number I read is: %d\n",MEM_R(src_index,R_DS));
	}
	if(cpu.flags.bits.df==0){
		IncDec=DATA_BYTE;
	}else{
		IncDec=-DATA_BYTE;	
	}
	reg_l(R_ESI)=reg_l(R_ESI)+IncDec;
	reg_l(R_EDI)=reg_l(R_EDI)+IncDec;
	print_asm_template1();
}

make_helper(concat3(instr,_,SUFFIX)){
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"
