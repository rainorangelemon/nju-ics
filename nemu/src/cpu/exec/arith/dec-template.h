#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	
        set_of(op_src->val,~((DATA_TYPE_S)(1)),DATA_BYTE,1);
        set_sf(result,DATA_BYTE);
        set_zf(result);
        set_pf(result);
        set_af(op_src->val,~((DATA_TYPE_S)(1)),1);	
	

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
