#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute (){
	if(DATA_BYTE==1){
		int8_t sum=~((int8_t)op_dest->val)+(int8_t)op_src->val;
		sum = sum +1-cpu.flags.bits.cf;
		OPERAND_W(op_dest,sum);
		set_of(op_dest->val,~((int8_t)op_src->val),1,1-cpu.flags.bits.cf);
		set_sf(sum,1);
		set_zf(sum);
		set_pf(sum);
		set_af(op_dest->val,~((int8_t)op_src->val),1-cpu.flags.bits.cf);	
		unsigned int result6=op_dest->val+~((int8_t)op_src->val)+1-cpu.flags.bits.cf;
	        unsigned int result7=((result6)<<23)>>31;
                cpu.flags.bits.cf=result7^1;
	}else if(DATA_BYTE==2){
		int16_t sum=(int16_t)op_dest->val+~((int16_t)op_src->val);
		sum = sum +1-cpu.flags.bits.cf;
		OPERAND_W(op_dest,sum);
		set_of(op_dest->val,~((int16_t)op_src->val),2,1-cpu.flags.bits.cf);
		set_sf(sum,2);
		set_zf(sum);
		set_pf(sum);
		set_af(op_dest->val,~((int16_t)op_src->val),1-cpu.flags.bits.cf);	
		unsigned int result6=op_dest->val+~((int16_t)op_src->val)+1-cpu.flags.bits.cf;
                unsigned int result7=((result6)<<15)>>31;
		cpu.flags.bits.cf=result7^1; 
	}else if(DATA_BYTE==4){
		int32_t sum=(int32_t)op_dest->val+~((int32_t)op_src->val);
		sum = sum +1-cpu.flags.bits.cf;
		OPERAND_W(op_dest,sum);
		set_of(op_dest->val,~op_src->val,4,1-cpu.flags.bits.cf);
		set_sf(sum,4);
		set_zf(sum);
		set_pf(sum);
		set_af(op_dest->val,~op_src->val,1-cpu.flags.bits.cf);	
		unsigned long long doctor1=(uint32_t)op_dest->val; 
                unsigned long long patient1=~(uint32_t)op_src->val; 
                unsigned long long result8=doctor1+patient1; 
                result8=result8+1-cpu.flags.bits.cf; 
                unsigned long long result7=((result8)<<31)>>63; 
                cpu.flags.bits.cf=result7^1;	     
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
