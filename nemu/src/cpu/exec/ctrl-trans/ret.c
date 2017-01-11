#include "cpu/exec/helper.h"

make_helper(ret_c3){
	if(ops_decoded.is_operand_size_16==true){
		uint32_t val=swaddr_read(reg_l(R_ESP),2,R_SS);
		reg_l(R_ESP)=reg_l(R_ESP)+2;
		cpu.eip=((cpu.eip)&0xffff0000)+val;
		cpu.eip=(cpu.eip)&0x0000ffff;	
	}else{
		int val=swaddr_read(reg_l(R_ESP),4,R_SS);
		reg_l(R_ESP)=reg_l(R_ESP)+4;
		cpu.eip=val;
	}
	cpu.eip-=1;
	return 1;
}

make_helper(ret_c2){
        int len=decode_i_w(eip+1);
	if(ops_decoded.is_operand_size_16==true){
		uint32_t val=swaddr_read(reg_l(R_ESP),2,R_SS);
                reg_l(R_ESP)=reg_l(R_ESP)+2;
                cpu.eip=((cpu.eip)&0xffff0000)+val;
                cpu.eip=(cpu.eip)&0x0000ffff;
        }else{  
                int val=swaddr_read(reg_l(R_ESP),4,R_SS);
                //reg_l(R_ESP)=reg_l(R_ESP)+4;
		cpu.eip=val;
        }
	cpu.eip-=len+1;
	reg_l(R_ESP)=reg_l(R_ESP)+op_src->val;
        return len+1;
}
