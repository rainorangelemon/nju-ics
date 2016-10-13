#include "cpu/exec/helper.h"

make_helper(cwtl_cltd){
	if(ops_decoded.is_operand_size_16){
		reg_w(R_EDX)=((int16_t)reg_w(R_EAX))>>15;
	}else{
		reg_l(R_EDX)=((int32_t)reg_l(R_EAX))>>31;
	}
	print_asm("cwtl");
	return 1;
}
