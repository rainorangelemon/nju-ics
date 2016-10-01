#include "cpu/exec/helper.h"

#define jcc_helper(name,length,condition) \
make_helper(concat4(j,name,_,length)) { \
	int len=concat(decode_i_,length) (eip+1); \
	if(condition==1){ \
		cpu.eip=cpu.eip+op_src->val; \
	} \
	return len+1; \
}

jcc_helper(e,b,cpu.flags.bits.zf)



#undef jcc_helper
