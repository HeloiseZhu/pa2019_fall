#include "cpu/instr.h"

static void instr_execute_1op() 
{
	OPERAND dest;
	operand_read(&opr_src);
	dest.type = OPR_MEM;
	dest.sreg = SREG_SS;	
	dest.data_size = data_size;
	cpu.esp -= (data_size / 8);
	dest.addr = cpu.esp;
	if(opr_src.data_size < data_size) {
		opr_src.val = ((int32_t)(opr_src.val << (32 - opr_src.data_size))) >> (32 - opr_src.data_size);
		opr_src.val = (0xffffffff >> (32 - opr_dest.data_size)) & opr_src.val;
	}
	dest.val = opr_src.val;
	operand_write(&dest);
}

make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
