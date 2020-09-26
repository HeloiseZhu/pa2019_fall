#include "cpu/instr.h"
#include "cpu/modrm.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}


make_instr_func(mov_c2r_l) {
	int len = 1;
	OPERAND c, r;
	c.data_size = 32;
	c.type = OPR_CREG;
	r.data_size = 32;
	r.type = OPR_REG;
	
	MODRM modrm;
	modrm.val = instr_fetch(eip + 1, 1);
	len += 1;
	c.addr = modrm.reg_opcode;
	r.addr = modrm.rm;
	
	operand_read(&c);
	r.val = c.val;
	operand_write(&r);

	print_asm_2("mov", "l", len + 1, &c, &r);
	return len;
}


make_instr_func(mov_r2c_l) {
	int len = 1;
	OPERAND r, c;
	r.data_size = 32;
	r.type = OPR_REG;
	c.data_size = 32;
	c.type = OPR_CREG;
	
	MODRM modrm;
	modrm.val = instr_fetch(eip + 1, 1);
	len += 1;
	c.addr = modrm.reg_opcode;
	r.addr = modrm.rm;
	
	operand_read(&r);
	c.val = r.val;
	operand_write(&c);

	print_asm_2("mov", "l", len + 1, &r, &c);
	return len;
}


make_instr_func(mov_rm2s_w) {
	int len = 1;
	uint8_t op_code;
	OPERAND rm, s;
	rm.data_size = 16;
	len += modrm_opcode_rm(eip + 1, &op_code, &rm);
	operand_read(&rm);
	s.type = OPR_SREG;
	s.addr = op_code;
	s.val = rm.val;
	s.data_size = 16;
	operand_write(&s);
	load_sreg(op_code);
	print_asm_2("mov", "w", len + 1, &rm, &s);
	return len;
}