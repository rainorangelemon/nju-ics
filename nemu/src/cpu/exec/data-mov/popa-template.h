#include "cpu/exec/template-start.h"
#define instr popa
#if DATA_BYTE==2
static DATA_TYPE pop_2(){
	DATA_TYPE temp=(DATA_TYPE)swaddr_read(reg_w(R_SP),2,R_SS);
	reg_w(R_SP)=reg_w(R_SP)+2;
	return temp;
}
#endif
#if DATA_BYTE==4
static DATA_TYPE pop_4(){
	DATA_TYPE temp=(DATA_TYPE)swaddr_read(reg_l(R_ESP),4,R_SS);
	reg_l(R_ESP)=reg_l(R_ESP)+4;
	return temp;
}
#endif

static void do_execute(){
#if DATA_BYTE==2		
	reg_w(R_DI)=pop_2();
	reg_w(R_SI)=pop_2();
	reg_w(R_BP)=pop_2();
	pop_2();
	reg_w(R_BX)=pop_2();
	reg_w(R_DX)=pop_2();
	reg_w(R_CX)=pop_2();
	reg_w(R_AX)=pop_2();
#endif
#if DATA_BYTE==4
	reg_l(R_EDI)=pop_4();
	reg_l(R_ESI)=pop_4();
	reg_l(R_EBP)=pop_4();
	pop_4();
	reg_l(R_EBX)=pop_4();
	reg_l(R_EDX)=pop_4();
	reg_l(R_ECX)=pop_4();
	reg_l(R_EAX)=pop_4();
#endif
	print_asm_template1();
}

make_instr_helper(zero);

#include "cpu/exec/template-end.h"
