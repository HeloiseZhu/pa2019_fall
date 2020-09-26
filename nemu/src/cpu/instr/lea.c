#include "cpu/instr.h"

make_instr_func(lea)
{
        OPERAND r, rm;
	r.data_size = 32;
	rm.data_size = 32;
	int len = 1;
	len += modrm_r_rm(eip+1, &r, &rm);
	r.val = rm.addr;
	operand_write(&r);
	print_asm_2("leal", "", len, &rm, &r);
        return len;
}
