#include "cpu/instr.h"

static void instr_execute_1op() 
{
	OPERAND st_top;
	st_top.type = OPR_MEM;
	st_top.sreg = SREG_SS;	
	st_top.data_size = data_size;
	st_top.addr = cpu.esp;
	operand_read(&st_top);
	cpu.esp += data_size / 8;
	opr_src.val = st_top.val;
	operand_write(&opr_src);
}

make_instr_impl_1op(pop, r, v)
//make_instr_impl_1op(pop, rm, v)
