#include "cpu/instr.h"

make_instr_func(ret_near) {
	OPERAND src;
	data_size = 32;
        src.type = OPR_MEM;
        src.sreg = SREG_SS;
        src.data_size = data_size;
        src.addr = cpu.esp;
        operand_read(&src);    
	cpu.esp += 4;
	print_asm_0("ret", "", 1);
        //cpu.eip = (0xffffffff >> (32 - data_size)) & src.val;
	cpu.eip = src.val;        
	return 0;
}


make_instr_func(ret_near_imm16) {
	//pop %eip
	OPERAND src;
	data_size = 32;
        src.type = OPR_MEM;
        src.sreg = SREG_SS;
        src.data_size = data_size;
        src.addr = cpu.esp;
        operand_read(&src);    
	cpu.esp += 4;
	
	//esp += imm16
	OPERAND imm;
	imm.type = OPR_IMM;
        imm.sreg = SREG_CS;
        imm.data_size = 16;
        imm.addr = eip + 1;
        operand_read(&imm);  	
	cpu.esp += imm.val;

	print_asm_0("ret", "", 1);
	cpu.eip = src.val;        
	return 0;
}

