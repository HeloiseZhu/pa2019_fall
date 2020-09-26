#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(int_)
{
	OPERAND intr_no;
    intr_no.type = OPR_IMM;
    intr_no.sreg = SREG_CS;
    intr_no.data_size = 8;
	intr_no.addr = cpu.eip + 1;
    operand_read(&intr_no);
	raise_sw_intr((uint8_t)intr_no.val);
	return 0;
}