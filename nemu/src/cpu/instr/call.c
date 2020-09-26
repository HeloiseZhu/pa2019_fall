#include "cpu/instr.h"

make_instr_func(call_near) {
	//push eip
	data_size = 32;
	OPERAND dest;
	dest.type = OPR_MEM;
	dest.sreg = SREG_SS;
	dest.data_size = data_size;
	cpu.esp -= 4;
	dest.addr = cpu.esp;
	dest.val = eip + 1 + data_size / 8;
	operand_write(&dest);
	

	OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;
        operand_read(&rel);
        int offset = sign_ext(rel.val, data_size);
        print_asm_1("call", "", 1 + data_size / 8, &rel);
	//printf("offset=%d\n",offset);
        cpu.eip += offset;
        return 1 + data_size / 8;
}


make_instr_func(call_near_indirect) {
	int len = 1;
	//push eip
	OPERAND dest;
	dest.type = OPR_MEM;
	dest.sreg = SREG_SS;
	dest.data_size = 32;
	cpu.esp -= 4;
	dest.addr = cpu.esp;
	//dest.val = eip + 2;
	//operand_write(&dest);
	
	OPERAND rm;
    rm.data_size = data_size;
    rm.addr = eip + 1;
    len += modrm_rm(eip + 1, &rm);
	operand_read(&rm);
    print_asm_1("call", "", len, &rm);
	
	dest.val = eip + len;
	operand_write(&dest);
	
    cpu.eip = rm.val;

    return 0;
}

