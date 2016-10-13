#include "cpu/exec/helper.h"

make_helper(cwtl_cltd){
	if(ops_decoded.is_operand_size_16){
		int16_t bits=reg_w(R_EAX);
		reg_w(R_EDX)=bits>>15;
	}else{
		int32_t bits=reg_l(R_EAX);
		reg_l(R_EDX)=bits>>31;
	}
	print_asm("cwtl");
	return 1;
}
