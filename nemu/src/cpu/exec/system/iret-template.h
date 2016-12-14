#include "cpu/exec/template-start.h"
#define instr iret
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
	cpu.eip=pop_2();
	printf("these violent delights have violent ends.\n");
#endif
#if DATA_BYTE==4
	cpu.eip=pop_4();
	seg(R_CS).selector.val=pop_4();
	cpu.flags.eflags=pop_4();
	seg(R_CS).dirty=false;
#endif
	print_asm_template1();
}

make_instr_helper(zero);

#include "cpu/exec/template-end.h"
