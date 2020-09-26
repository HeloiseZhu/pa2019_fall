#include "cpu/instr.h"

make_instr_func(iret) {
	cpu.eip = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	uint16_t data = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.cs.val = data;
	cpu.esp += 4;
	/*
	uint32_t data_m = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	uint32_t data_l = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	cpu.cs.val = data_l & 0xffff;
	cpu.cs.base = (data_m & 0xffff) + (data_l >> 16);
	cpu.cs.limit = (data_h & 0xffff) + (data_m >> 16);
	cpu.cs.type = (data_h >> 16);
	cpu.cs.privilege_level = (data_h >> 21);
	cpu.cs.soft_use = (data_h >> 23);*/
	cpu.eflags.val = vaddr_read((vaddr_t)cpu.esp, SREG_SS, 4);
	cpu.esp += 4;
	return 0;
}