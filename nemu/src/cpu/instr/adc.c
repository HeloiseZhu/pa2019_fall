#include "cpu/instr.h"

static void instr_execute_2op()
{
	operand_read(&opr_dest);
	operand_read(&opr_src);
	if(opr_src.data_size != opr_dest.data_size) {
		opr_src.val = ((int32_t)(opr_src.val << (32 - opr_src.data_size))) >> (32 - opr_src.data_size);
		opr_src.val = (0xffffffff >> (32 - opr_dest.data_size)) & opr_src.val;
	}
	opr_dest.val = alu_adc(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(adc, r, rm, b)
make_instr_impl_2op(adc, r, rm, v)
make_instr_impl_2op(adc, rm, r, b)
make_instr_impl_2op(adc, rm, r, v)
make_instr_impl_2op(adc, i, a, b)
make_instr_impl_2op(adc, i, a, v)
make_instr_impl_2op(adc, i, rm, b)
make_instr_impl_2op(adc, i, rm, v)
make_instr_impl_2op(adc, i, rm, bv)
