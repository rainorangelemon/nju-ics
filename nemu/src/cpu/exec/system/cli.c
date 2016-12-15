#include "cpu/exec/helper.h"

make_helper(cli){
	cpu.flags.bits.ief=0;
	print_asm("cli");
	return 1;
}
