#include "cpu/exec/helper.h"

make_helper(sti){
	cpu.flags.bits.ief=1;
	print_asm("sti");
	return 1;
}
