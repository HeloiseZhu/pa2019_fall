#include "cpu/cpu.h"
#include <stdlib.h>

// CF contains information relevant to unsigned integers
void set_CF_add(uint32_t result, uint32_t src, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = (result < src);
}

void set_ZF(uint32_t result, size_t data_size) {
	result = result & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.ZF = (result == 0);
}

// SF and OF contain information relevant to signed integers
void set_SF(uint32_t result, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF= sign(result);
}

void set_PF(uint32_t result) {
	result = result & 0x000000FF;
	uint32_t val = 0;
	while(result != 0) {
		val ^= result;
		result = (result >> 1);
	}
	val = val & 0x1;
	cpu.eflags.PF = (val == 0);
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
		case 8: 
			result = sign_ext(result & 0xFF, 8); 
			src= sign_ext(src& 0xFF, 8); 
			dest= sign_ext(dest& 0xFF, 8); 
			break;
		case 16: 
			result = sign_ext(result & 0xFFFF, 16); 
			src= sign_ext(src & 0xFFFF, 16); 
			dest= sign_ext(dest & 0xFFFF, 16); 
			break;
		default: break;// do nothing
	}
	if(sign(src) == sign(dest)) {
		if(sign(src) != sign(result))
			cpu.eflags.OF= 1;
		else
			cpu.eflags.OF= 0;
	} else {
		cpu.eflags.OF= 0;
	}
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
/* Golden Version
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	uint32_t res = 0;
	res = dest + src;
	set_CF_add(res, src, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	uint32_t res = 0;
	uint32_t val_CF_0 = cpu.eflags.CF;
	res = dest + src + val_CF_0;
	set_CF_add(dest + src, src, data_size);
	uint32_t val_CF_1 = cpu.eflags.CF;
	set_CF_add(res, val_CF_0, data_size);
	cpu.eflags.CF = (cpu.eflags.CF || val_CF_1);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(dest + src, src, dest, data_size);
	uint32_t val_OF_1 = cpu.eflags.OF;
	set_OF_add(res, val_CF_0, dest + src, data_size);
	cpu.eflags.OF = (cpu.eflags.OF ^ val_OF_1);
	return res & (0xFFFFFFFF >> (32 - data_size));
}


void set_CF_sub(uint32_t src, uint32_t dest, size_t data_size) {
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src= sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = (dest < src);
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size){
	switch(data_size) {
		case 8: 
			result = sign_ext(result & 0xFF, 8); 
			src= sign_ext(src& 0xFF, 8); 
			dest= sign_ext(dest& 0xFF, 8); 
			break;
		case 16: 
			result = sign_ext(result & 0xFFFF, 16); 
			src= sign_ext(src & 0xFFFF, 16); 
			dest= sign_ext(dest & 0xFFFF, 16); 
			break;
		default: break;// do nothing
	}
	if(sign(src) != sign(dest)) {
		if(sign(dest) != sign(result))
			cpu.eflags.OF= 1;
		else
			cpu.eflags.OF= 0;
	} else {
		cpu.eflags.OF= 0;
	}
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU 
	return __ref_alu_sub(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	uint32_t res = 0;
	res = dest - src;
	set_CF_sub(src, dest, data_size);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sub(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	uint32_t res = 0;
	uint32_t val_CF_0 = cpu.eflags.CF;
	res = dest - src - val_CF_0;
	set_CF_sub(src, dest, data_size);
	uint32_t val_CF_1 = cpu.eflags.CF;
	set_CF_sub(val_CF_0, dest - src, data_size);
	cpu.eflags.CF = (cpu.eflags.CF || val_CF_1);
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sub(dest - src, src, dest, data_size);
	uint32_t val_OF_1 = cpu.eflags.OF;
	set_OF_sub(res, val_CF_0, dest - src, data_size);
	cpu.eflags.OF = (cpu.eflags.OF ^ val_OF_1);
	return res & (0xFFFFFFFF >> (32 - data_size));
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	uint64_t dest_ext = dest & (0xFFFFFFFFFFFFFFFF >> (64 - data_size));
	uint64_t src_ext = src & (0xFFFFFFFFFFFFFFFF >> (64 - data_size));
	uint64_t res_ext = dest_ext * src_ext;
	cpu.eflags.CF = ((res_ext >> data_size) != 0);
	cpu.eflags.OF = cpu.eflags.CF;
	return res_ext;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	uint64_t dest_ext = sign_ext_64(dest & (0xFFFFFFFFFFFFFFFF >> (64 - data_size)), data_size);
	uint64_t src_ext = sign_ext_64(src & (0xFFFFFFFFFFFFFFFF >> (64 - data_size)), data_size);
	uint64_t res_ext = dest_ext * src_ext;
	return res_ext;
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	if(src == 0) abort();
	uint64_t res_ext = (dest / src) & 0x00000000FFFFFFFF;
	uint32_t res = (uint32_t)res_ext;
	return res;
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	if(src == 0) abort();
	int64_t res_ext = (dest / src) & 0x00000000FFFFFFFF;
	int32_t res = (int32_t)res_ext;
	return res;
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	uint64_t res_ext = (dest % src) & 0x00000000FFFFFFFF;
	uint32_t res = (uint32_t)res_ext;
	return res;
}

int32_t alu_imod(int64_t src, int64_t dest)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	int64_t res_ext = (dest % src) & 0x00000000FFFFFFFF;
	int32_t res = (int32_t)res_ext;
	return res;
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	src = src & (0xFFFFFFFF >> (32 - data_size));
	uint32_t res = (dest & src);
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res;
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	src = src & (0xFFFFFFFF >> (32 - data_size));
	uint32_t res = (dest ^ src);
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res;
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	src = src & (0xFFFFFFFF >> (32 - data_size));
	uint32_t res = (dest | src);
	cpu.eflags.CF = 0;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res;
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	src = src & (0xFFFFFFFF >> (32 - data_size));
	uint32_t res = (dest << src);
	cpu.eflags.CF = (dest >> (data_size - src)) & 0x1;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	src = src & (0xFFFFFFFF >> (32 - data_size));
	uint32_t res = (dest >> src);
	cpu.eflags.CF = (dest >> (src - 1)) & 0x1;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	dest = dest & (0xFFFFFFFF >> (32 - data_size));
	src = src & (0xFFFFFFFF >> (32 - data_size));
	uint32_t sym = (dest >> (data_size - 1)) << (data_size - 1);
	uint32_t i = src, res = dest;	
	while(i) {
		res = (res >> 1) | sym;		
		i--;	
	}
	cpu.eflags.CF = (dest >> (src - 1)) & 0x1;
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	return res;
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
/*
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
*/
	return alu_shl(src, dest, data_size);
}
