#include "cpu/exec/template-start.h"
#define instr pusha
#if DATA_BYTE==2
static void push_2(int data){
	reg_w(R_SP)=reg_w(R_SP)-2;
	swaddr_write(reg_w(R_SP),2,data,R_SS);
}
#endif
#if DATA_BYTE==4
static void push_4(int data){
	reg_l(R_ESP)=reg_l(R_ESP)-4;
	swaddr_write(reg_l(R_ESP),4,data,R_SS);
}
#endif

static void do_execute(){
	DATA_TYPE temp;
#if DATA_BYTE==2
	temp=reg_w(R_SP);
	push_2(reg_w(R_AX));
	push_2(reg_w(R_CX));
	push_2(reg_w(R_DX));
	push_2(reg_w(R_BX));
	push_2(temp);
	push_2(reg_w(R_BP));
	push_2(reg_w(R_SI));
	push_2(reg_w(R_DI));
#endif
#if DATA_BYTE==4
	temp=reg_l(R_ESP);
	push_4(reg_l(R_EAX));
	push_4(reg_l(R_ECX));
	push_4(reg_l(R_EDX));
	push_4(reg_l(R_EBX));
	push_4(temp);
	push_4(reg_l(R_EBP));
	push_4(reg_l(R_ESI));
	push_4(reg_l(R_EDI));
#endif
	print_asm_template1();
}

make_instr_helper(zero);

#include "cpu/exec/template-end.h"
