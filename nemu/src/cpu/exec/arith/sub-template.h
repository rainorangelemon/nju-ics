#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute (){
	if(DATA_BYTE==1){
		int8_t dest_val=(int8_t)op_dest->val;
		int8_t src_val=(int8_t)op_src->val;
		src_val=~src_val;
		int8_t sum=dest_val+src_val+1;
		OPERAND_W(op_dest,sum);
		set_of(dest_val,src_val,1,1);
		set_sf(sum,1);
		set_zf(sum);
		set_pf(sum);
		set_cf(dest_val,src_val,1,1);
		set_af(dest_val,src_val,1);	
	}else if(DATA_BYTE==2){
		int16_t dest_val=(int16_t)op_dest->val;
		int16_t src_val=(int16_t)op_src->val;
		src_val=~src_val;
		int16_t sum=dest_val+src_val+1;
		OPERAND_W(op_dest,sum);
		set_of(dest_val,src_val,2,1);
		set_sf(sum,2);
		set_zf(sum);
		set_pf(sum);
		set_cf(dest_val,src_val,1,2);
		set_af(dest_val,src_val,1);	
	}else if(DATA_BYTE==4){
		int32_t dest_val=(int32_t)op_dest->val;
		int32_t src_val=(int32_t)op_src->val;
		src_val=~src_val;
		int32_t sum=dest_val+src_val+1;
		OPERAND_W(op_dest,sum);
		set_of(dest_val,src_val,4,1);
		set_sf(sum,4);
		set_zf(sum);
		set_pf(sum);
		set_cf(dest_val,src_val,1,4);
		set_af(dest_val,src_val,1);	
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
