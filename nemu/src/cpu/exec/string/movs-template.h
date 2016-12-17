#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute(){
	int IncDec=0;
	uint32_t src_index=reg_l(R_ESI);
	uint32_t dest_index=reg_l(R_EDI);
	MEM_W(dest_index,MEM_R(src_index,R_DS),R_ES);
	printf("esi:0x%x ----->  edi:0x%x  0x%x\n",src_index,dest_index,MEM_R(src_index,R_DS));
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
