#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

make_helper_v(jmp_rel)
make_helper_v(jmp_rm)

void load_seg(uint8_t sreg);

make_helper(ljmp){
	unsigned short dest=instr_fetch(cpu.eip+5,2);
	swaddr_t jump=instr_fetch(cpu.eip+1,4);
	seg(R_CS).selector.val=dest;
	seg(R_CS).dirty=false;
	load_seg(R_CS);
	cpu.eip=jump-7;
	return 7;
}
