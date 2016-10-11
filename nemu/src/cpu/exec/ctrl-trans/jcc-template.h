#include "cpu/exec/template-start.h"

#define instr jcc

#define jcc_helper(name,condition) \
make_helper(concat4(j,name,_,SUFFIX)) { \
	int len=instr_fetch(eip+1,DATA_BYTE); \
	if(condition){ \
		cpu.eip=cpu.eip+op_src->val; \
	} \
	if(ops_decoded.is_operand_size_16==true) \
		cpu.eip=((cpu.eip+len+1)&(0xffff))-len-1; \
	print_asm_template1(); \
	return DATA_BYTE+1; \
}

jcc_helper(a,(cpu.flags.bits.cf==0)&&(cpu.flags.bits.zf==0))
jcc_helper(ae,cpu.flags.bits.cf==0)
jcc_helper(b,cpu.flags.bits.cf==1)
jcc_helper(be,(cpu.flags.bits.cf==1)||(cpu.flags.bits.zf==1))
jcc_helper(c,cpu.flags.bits.cf==1)
jcc_helper(cxz,((reg_w(R_ECX)==0)&&(ops_decoded.is_operand_size_16==true))||((reg_l(R_ECX)==0)&&(ops_decoded.is_operand_size_16==false)))
jcc_helper(e,cpu.flags.bits.zf)
jcc_helper(g,(cpu.flags.bits.zf==0)&&(cpu.flags.bits.sf==cpu.flags.bits.of))
jcc_helper(ge,cpu.flags.bits.sf==cpu.flags.bits.of)
jcc_helper(l,cpu.flags.bits.sf!=cpu.flags.bits.of)
jcc_helper(le,(cpu.flags.bits.zf==1)||(cpu.flags.bits.sf!=cpu.flags.bits.of))
jcc_helper(na,(cpu.flags.bits.cf==1)||(cpu.flags.bits.zf==1))
jcc_helper(nae,(cpu.flags.bits.cf==1))
jcc_helper(nb,(cpu.flags.bits.cf==0))
jcc_helper(nbe,(cpu.flags.bits.cf==0)&&(cpu.flags.bits.zf==0))
jcc_helper(nc,cpu.flags.bits.cf==0)
jcc_helper(ne,cpu.flags.bits.zf==0)
jcc_helper(ng,(cpu.flags.bits.zf==1)||(cpu.flags.bits.sf!=cpu.flags.bits.of))
jcc_helper(nge,cpu.flags.bits.sf!=cpu.flags.bits.of)
jcc_helper(nl,cpu.flags.bits.sf==cpu.flags.bits.of)
jcc_helper(nle,(cpu.flags.bits.zf==0)&&(cpu.flags.bits.sf==cpu.flags.bits.of))
jcc_helper(no,cpu.flags.bits.of==0)
jcc_helper(np,cpu.flags.bits.pf==0)
jcc_helper(ns,cpu.flags.bits.sf==0)
jcc_helper(nz,cpu.flags.bits.zf==0)
jcc_helper(o,cpu.flags.bits.of==1)
jcc_helper(p,cpu.flags.bits.pf==1)
jcc_helper(s,cpu.flags.bits.sf==1)
jcc_helper(z,cpu.flags.bits.zf==1)
jcc_helper(pe,cpu.flags.bits.pf==1)	
jcc_helper(po,cpu.flags.bits.pf==0)

#undef jcc_helper

#include "cpu/exec/template-end.h"
