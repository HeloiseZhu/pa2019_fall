#include "cpu/instr.h"
#include "device/port_io.h"

make_instr_func(out_b)
{
	uint16_t port = cpu.edx & 0xffff;
	uint32_t data = cpu.eax & 0xff;
	pio_write(port, 1, data);
	return 1;
}


make_instr_func(out_v)
{
	uint16_t port = cpu.edx & 0xffff;
	uint32_t data = cpu.eax & (0xffffffff >> (32 - data_size));
	pio_write(port, data_size / 8, data);
	return 1;
}