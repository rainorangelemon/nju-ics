#include "cpu/exec/helper.h"

make_helper(hlt){
	/*I think this out during a long time, at last I asked my classmates... This way is so smart!*/
	if(cpu.INTR==true)
		return 1;
	else
		return 0;
}
