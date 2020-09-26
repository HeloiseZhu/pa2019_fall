#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_near_indirect)
{	
        OPERAND rm;
        rm.data_size = data_size;
        rm.addr = eip + 1;
        modrm_rm(eip + 1, &rm);
	operand_read(&rm);
        print_asm_1("jmp", "", 1 + data_size / 8, &rm);
        cpu.eip = rm.val;

        return 0;
}


make_instr_func(jmp_short)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, 8);
        print_asm_1("jmp", "", 2, &rel);

        cpu.eip += offset;

        return 2;
}


make_instr_func(ljmp) {
	OPERAND _eip, _cs;
	_eip.type = OPR_IMM;
	_eip.data_size = data_size;
	_eip.sreg = SREG_CS;
	_eip.addr = eip + 1;
	operand_read(&_eip);
	
	_cs.type = OPR_IMM;
	_cs.data_size = 16;
	_cs.sreg = SREG_CS;
	_cs.addr = eip + 1 + data_size / 8;
	operand_read(&_cs);
	cpu.cs.val = _cs.val;
	
	load_sreg(1); //segReg[1]--cs
	
	print_asm_2("ljmp", "", 3 + data_size / 8, &_cs, &_eip);
	cpu.eip = _eip.val;
	return 0;
}