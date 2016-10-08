#include "cpu/exec/template-start.h"

#define instr add

static void do_execute (){
	if(DATA_BYTE==1){
		int8_t sum=(int8_t)op_dest->val+(int8_t)op_src->val;
		OPERAND_W(op_dest,sum);
		set_of(op_dest->val,op_src->val,1,0);
		set_sf(sum,1);
		set_zf(sum);
		set_pf(sum);
		set_cf(op_dest->val,op_src->val,0,1);
		set_af(op_dest->val,op_src->val,0);	
	}else if(DATA_BYTE==2){
		int16_t sum=(int16_t)op_dest->val+(int16_t)op_src->val;
		OPERAND_W(op_dest,sum);
		set_of(op_dest->val,op_src->val,2,0);
		set_sf(sum,2);
		set_zf(sum);
		set_pf(sum);
		set_cf(op_dest->val,op_src->val,0,2);
		set_af(op_dest->val,op_src->val,0);
	}else if(DATA_BYTE==4){
		int32_t sum=op_dest->val+op_src->val;
		OPERAND_W(op_dest,sum);
		set_of(op_dest->val,op_src->val,4,0);
		set_sf(sum,4);
		set_zf(sum);
		set_pf(sum);
		set_cf(op_dest->val,op_src->val,0,4);
		set_af(op_dest->val,op_src->val,0);
	}
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if ((DATA_BYTE==2)||(DATA_BYTE==4))
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
