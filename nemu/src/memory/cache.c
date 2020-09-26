#include "cpu/cpu.h"
#include "memory/memory.h"
#include "memory/cache.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

CacheLine L1_dcache[1024];


void init_cache() {
	for(int i = 0; i < 1024; i++) {
		L1_dcache[i].valid_bit = false;
	}
}

uint32_t cache_read(paddr_t paddr, size_t len, CacheLine* cache) {
	uint32_t sign = (paddr >> 13) & 0x7ffff;				//19 bits m-q | m==26, c==10, q==7, s==3 ,b==6
	uint32_t group_index = ((paddr >> 6) & 0x7f) % 128;		//7  bits q
	uint32_t in_blk_addr = paddr & 0x3f;					//6  bits b
	uint32_t res = 0;
	
	//printf("In function cache_read(), paddr=%x, len=%d, sign=%d, group=%d, in_blk_addr=%x\n", (unsigned)paddr, len, sign, group_index, in_blk_addr);
	
	bool cmp_res = false;
	for(int i = 0; i < 8; i++) {
		int target = 8 * group_index + i;
		//Hit
		if(cache[target].sign == sign && cache[target].valid_bit) { 
			cmp_res = true;
			if(len + in_blk_addr <= 64) {
				//little endian 
				for(int j = len - 1; j >= 0; j--) { 
					res = (res << 8) + cache[target].data[in_blk_addr + j];
				}
			}
			else {
				//cross lines
				size_t len_l = 64 - in_blk_addr;
				size_t len_h = len - len_l;
				uint32_t data_l = cache_read(paddr, len_l, cache);
				uint32_t data_h = cache_read(paddr + len_l, len_h, cache);
				res = (data_h << (8 * len_l)) + data_l;
			}
		}
	}
	
	//Miss
	if(cmp_res == false) {
		uint32_t blk_addr = paddr & 0xffffffc0;	
		int index = 8 * group_index +  (rand() % 8);
		for(int i = 0; i < 64; i++) {
			uint8_t dat_r = hw_mem_read(blk_addr + i, 1);
			cache[index].data[i] = dat_r;
		}
		cache[index].valid_bit = true;
		cache[index].sign = sign; 
		if(len + in_blk_addr <= 64) {
			res = cache_read(paddr, len, cache);
		}
		else {
			size_t len_l = 64 - in_blk_addr;
			size_t len_h = len - len_l;
			uint32_t data_l = cache_read(paddr, len_l, cache);
			uint32_t data_h = cache_read(paddr + len_l, len_h, cache);
			res = (data_h << (8 * len_l)) + data_l;
		}
	}
	
	return res;
}

void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine* cache) {
	uint32_t sign = (paddr >> 13) & 0x7ffff;				//19 bits m-q | m==26, c==10, q==7, s==3 ,b==6
	uint32_t group_index = ((paddr >> 6) & 0x7f) % 128;		//7  bits q
	uint32_t in_blk_addr = paddr & 0x3f;					//6  bits b
	
	bool cmp_res = false;
	for(int i = 0; i < 8; i++) {
		int target = 8 * group_index + i;
		//Hit
		if(cache[target].sign == sign && cache[target].valid_bit) { 
			cmp_res = true;
			if(len + in_blk_addr <= 64) {
				uint32_t tempdat = data;
				for(int j = 0; j < len; j++) {
					cache[target].data[in_blk_addr + j] = tempdat & 0xff;
					tempdat >>= 8;
				}
				hw_mem_write(paddr, len, data);
			}
			else {
				size_t len_l = 64 - in_blk_addr;
				size_t len_h = len - len_l;
				uint32_t data_l = data & (0xffffffff >> (32 - len_l * 8));
				uint32_t data_h = data >> (8 * len_l);
				cache_write(paddr, len_l, data_l, cache);
				cache_write(paddr + len_l, len_h, data_h, cache);
			}
		}
	}
	
	if(cmp_res == false) {
		hw_mem_write(paddr, len, data);
	}
}