#include "cpu/instr.h"

make_instr_func(leave)
{
	//mov %esp, %ebp
	cpu.esp = cpu.ebp;

	//pop %ebp
        OPERAND opr_esp;
	opr_esp.type = OPR_MEM;
	opr_esp.sreg = SREG_SS;
	opr_esp.data_size = 0x20;
	opr_esp.addr = cpu.esp;
	operand_read(&opr_esp);
	cpu.ebp = opr_esp.val;
	cpu.esp += 4;
	
        return 1;
}
