#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(in_b)
{
	uint16_t port = cpu.edx & 0xffff;
	cpu.eax = pio_read(port, 1);
	return 1;
}


make_instr_func(in_v)
{
	uint16_t port = cpu.edx & 0xffff;
	//uint32_t data = (cpu.eax & (0xffffffff << data_size)) + pio_read(port, data_size / 8);
	//cpu.eax = data;
	cpu.eax = pio_read(port, data_size / 8);
	return 1;
}