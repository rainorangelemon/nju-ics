#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

make_helper_v(push_r)
make_helper_v(push_i)
make_helper_v(push_rm)

make_helper(push_si_b){
	int result=(int)(int8_t)(uint8_t)instr_fetch(eip+1,1);
	reg_l(R_ESP)=reg_l(R_ESP)-4;
	swaddr_write(reg_l(R_ESP),4,result,R_SS);
        print_asm("push_si_b");
	return 2;
}
