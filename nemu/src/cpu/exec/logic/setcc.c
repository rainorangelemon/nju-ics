#include "cpu/exec/helper.h"

#define setcc_helper(name,condition) \
make_helper(concat(set,name)){ \
	int len1=decode_rm_b(eip+1); \
	if(condition){ \
		write_operand_b(op_src,1); \
	} \
       	else { \
		write_operand_b(op_src,0); \
	} \
	return len1+1; \
}

setcc_helper(a,(cpu.flags.bits.cf==0)&&(cpu.flags.bits.zf==0))
setcc_helper(ae,cpu.flags.bits.cf==0)
setcc_helper(b,cpu.flags.bits.cf==1)
setcc_helper(be,(cpu.flags.bits.cf==1)||(cpu.flags.bits.zf==1))
setcc_helper(c,cpu.flags.bits.cf==1)
setcc_helper(e,cpu.flags.bits.zf==1)
setcc_helper(g,(cpu.flags.bits.zf==0)&&(cpu.flags.bits.sf==cpu.flags.bits.of))
setcc_helper(ge,cpu.flags.bits.sf==cpu.flags.bits.of)
setcc_helper(l,cpu.flags.bits.sf!=cpu.flags.bits.of)
setcc_helper(le,(cpu.flags.bits.zf==1)||(cpu.flags.bits.sf!=cpu.flags.bits.of))
setcc_helper(na,(cpu.flags.bits.cf==1)||(cpu.flags.bits.zf==1))
setcc_helper(nae,cpu.flags.bits.cf==1)
setcc_helper(nb,cpu.flags.bits.cf==0)
setcc_helper(nbe,(cpu.flags.bits.cf==0)&&(cpu.flags.bits.zf==0))
setcc_helper(nc,cpu.flags.bits.cf==0)
setcc_helper(ne,cpu.flags.bits.zf==0)
setcc_helper(ng,(cpu.flags.bits.zf==1)||(cpu.flags.bits.sf!=cpu.flags.bits.of))
setcc_helper(nge,cpu.flags.bits.sf!=cpu.flags.bits.of)
setcc_helper(nl,cpu.flags.bits.sf==cpu.flags.bits.of)
setcc_helper(nle,(cpu.flags.bits.zf==0)&&(cpu.flags.bits.sf==cpu.flags.bits.of))
setcc_helper(no,cpu.flags.bits.of==0)
setcc_helper(np,cpu.flags.bits.pf==0)
setcc_helper(ns,cpu.flags.bits.sf==0)
setcc_helper(nz,cpu.flags.bits.zf==0)
setcc_helper(o,cpu.flags.bits.of==1)
setcc_helper(p,cpu.flags.bits.pf==1)
setcc_helper(pe,cpu.flags.bits.pf==1)
setcc_helper(po,cpu.flags.bits.pf==0)
setcc_helper(s,cpu.flags.bits.sf==1)
setcc_helper(z,cpu.flags.bits.zf==1)

#undef setcc_helper
