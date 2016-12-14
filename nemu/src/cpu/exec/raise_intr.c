#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/exec/helper.h"
#include <setjmp.h>
extern jmp_buf jbuf;

void raise_intr(uint8_t NO) { 
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 * That is, use ''NO'' to index the IDT.
	 *  */
	uint32_t temp[2];
	GateDesc *gate=(GateDesc *)temp;
	reg_l(R_ESP)=reg_l(R_ESP)-4;
	swaddr_write(reg_l(R_ESP),4,(uint32_t)cpu.flags.eflags,R_SS);
	reg_l(R_ESP)=reg_l(R_ESP)-4;
	swaddr_write(reg_l(R_ESP),4,(uint32_t)seg(R_CS).selector.val,R_SS);
	reg_l(R_ESP)=reg_l(R_ESP)-4;
	swaddr_write(reg_l(R_ESP),4,(uint32_t)(cpu.eip),R_SS);
	Assert(NO*8<=cpu.idtr.limit,"exceed\n");
	lnaddr_t intaddr=(lnaddr_t)cpu.idtr.base+8*NO;
	temp[0]=lnaddr_read(intaddr,4);
	temp[1]=lnaddr_read(intaddr+4,4);
	Assert(gate->present==1,"raise_intr:GateDesc's is not presented\n");
	uint16_t seg=gate->segment;
	uint32_t off=(gate->offset_31_16<<16)+gate->offset_15_0;
	cpu.eip=off;
	seg(R_CS).selector.val=seg;
	seg(R_CS).dirty=false;
	if(cpu.cr0.protect_enable==0){
		cpu.flags.bits.ief=0;
		cpu.flags.bits.tf=0;
	}
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}
