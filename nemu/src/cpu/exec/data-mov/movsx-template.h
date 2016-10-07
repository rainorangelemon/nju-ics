#include "cpu/exec/helper.h"

#include "cpu/decode/modrm.h"

make_helper(movsx_rmb2r_w){
	op_src->size = 1;
	int len = read_ModR_M(eip+1, op_src, op_dest);
	op_dest->val = reg_w(op_dest->reg);
	int16_t lowbit = (int16_t)(int8_t)(op_src->val);
	write_operand_w(op_dest,lowbit);
	print_asm_template2();
	return len+1;
}

make_helper(movsx_rmb2r_l){
	op_src->size = 1;
	int len = read_ModR_M(eip+1, op_src, op_dest);
	op_dest->val = reg_l(op_dest->reg);
	int32_t lowbit = (int32_t)(int8_t)(op_src->val);
	write_operand_l(op_dest,lowbit);
	print_asm_template2();
	return len+1;
}

make_helper(movsx_rmw2r){
	op_src->size = 2;
	int len = read_ModR_M(eip+1, op_src, op_dest);
	op_dest->val = reg_l(op_dest->reg);
	int32_t lowbit = (int32_t)(int8_t)(op_src->val);
	write_operand_l(op_dest,lowbit);
	print_asm_template2();
	return len+1;
}
