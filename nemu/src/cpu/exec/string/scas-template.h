#include "cpu/exec/template-start.h"

#define instr scas

static void do_execute(){
	int IncDec=0;
	uint32_t src_index=reg_l(R_EDI);
	DATA_TYPE result=MEM_R(src_index);
	result=~result;
	DATA_TYPE sum=result+1+REG(R_EAX);
	set_of(REG(R_EAX),result,DATA_BYTE,1);
	set_sf(sum,DATA_BYTE);
	set_zf(sum);
	set_pf(sum);
	set_cf(REG(R_EAX),result,1,DATA_BYTE);
	set_af(REG(R_EAX),result,1);
	if(cpu.flags.bits.df==0){
		IncDec=DATA_BYTE;
	}else{
		IncDec=-DATA_BYTE;	
	}
	reg_l(R_EDI)=reg_l(R_EDI)+IncDec;
}

make_helper(concat3(instr,_,SUFFIX)){
	do_execute();
	return 1;
}

#include "cpu/exec/template-end.h"
