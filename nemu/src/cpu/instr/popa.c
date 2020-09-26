#include "cpu/instr.h"

make_instr_func(popa) 
{
	cpu.edi = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	cpu.esi = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	cpu.ebp = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	cpu.ebx = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	cpu.edx = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	cpu.ecx = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	cpu.eax = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	return 1;
}
