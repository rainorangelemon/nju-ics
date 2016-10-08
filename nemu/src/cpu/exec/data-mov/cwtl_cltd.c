#include "cpu/exec/helper.h"

make_helper(cwtl_cltd){
	if(ops_decoded.is_operand_size_16){
		if(reg_w(R_EAX)<0){
			reg_w(R_EDX)=0xffff;
		}else{
			reg_w(R_EDX)=0;
		}
	}else{
		if(reg_l(R_EAX)<0){
			reg_l(R_EDX)=0xffffffff;
		}else{
			reg_l(R_EDX)=0;
		}
	}
	return 1;
}
