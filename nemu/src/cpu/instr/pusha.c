#include "cpu/instr.h"

make_instr_func(pusha) 
{
	uint32_t len = data_size / 8;
	uint32_t Temp = vaddr_read((vaddr_t)cpu.esp, SREG_SS, len);
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, len, cpu.eax);
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, len, cpu.ecx);
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, len, cpu.edx);
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, len, cpu.ebx);
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, len, Temp);
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, len, cpu.ebp);
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, len, cpu.esi);
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, len, cpu.edi);
	return 1;
}