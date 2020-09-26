#include "cpu/instr.h"

make_instr_func(lidt) {
	int len = 1;
	OPERAND m16, m32;
	m16.data_size = 16;
	len += modrm_rm(eip + 1, &m16);
	operand_read(&m16);
	
	m32.data_size = 32;
	m32.type = OPR_MEM;
	m32.sreg = m16.sreg;
	m32.addr = m16.addr + 2;
	operand_read(&m32);
	if(data_size == 16)
		m32.val &= 0xffffff;
	
	cpu.idtr.limit = m16.val;
	cpu.idtr.base = m32.val;
	//printf("limit:%x, base:%x\n",m16.val,m32.val);
	print_asm_2("lidt", "", len + 1, &m16, &m32);
	return len;
}