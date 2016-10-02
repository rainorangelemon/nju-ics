#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	if(DATA_BYTE==1){
		uint8_t op_dest1=op_dest->val;
		uint8_t op_src1=op_src->val;
		uint8_t output = (~op_src1)+op_dest1+1;
		op_src1 = (~op_src1);	
		set_of(op_src1,op_dest1,DATA_BYTE,1);
		set_sf(output,DATA_BYTE);
		set_zf(output);
		set_pf(output);
		set_cf(op_src1,op_dest1,1,DATA_BYTE);
		set_af(op_src1,op_dest1,1);
	}else if(DATA_BYTE==2){
		uint16_t op_dest1=op_dest->val;
		uint16_t op_src1=op_src->val;
		uint16_t output = (~op_src1)+op_dest1+1;
		op_src1 = (~op_src1);	
		set_of(op_src1,op_dest1,DATA_BYTE,1);
		set_sf(output,DATA_BYTE);
		set_zf(output);
		set_pf(output);
		set_cf(op_src1,op_dest1,1,DATA_BYTE);
		set_af(op_src1,op_dest1,1);
	}else if(DATA_BYTE==4){
		uint32_t op_dest1=op_dest->val;
		uint32_t op_src1=op_src->val;
		uint32_t output = (~op_src1)+op_dest1+1;
		op_src1 = (~op_src1);	
		set_of(op_src1,op_dest1,DATA_BYTE,1);
		set_sf(output,DATA_BYTE);
		set_zf(output);
		set_pf(output);
		set_cf(op_src1,op_dest1,1,DATA_BYTE);
		set_af(op_src1,op_dest1,1);
	}
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE==2 ||DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
