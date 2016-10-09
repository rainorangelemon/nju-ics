#include "cpu/exec/helper.h"

make_helper(leave){
	reg_l(R_ESP)=reg_l(R_EBP);
	if(ops_decoded.is_operand_size_16==true){
		uint32_t val =swaddr_read(reg_l(R_ESP),2);
		reg_l(R_ESP)=reg_l(R_ESP)+2;
		reg_w(R_EBP)=val;	
	}else{
		int val = swaddr_read(reg_l(R_ESP),4);
		reg_l(R_ESP)=reg_l(R_ESP)+4;
		reg_l(R_EBP)=val;
	}	
	return 1;
}
