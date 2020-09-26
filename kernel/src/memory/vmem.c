#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem

//PTE vptable[NR_PT] align_to_page; // videl page tables
PTE vptable[NR_PTE] align_to_page; // videl page tables

PDE *get_updir();

void create_video_mapping()
{

	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	
	/*
	PDE *pdir = get_updir();
	PTE *ptable = (PTE *)(vptable + (VMEM_ADDR >> 12));
	uint32_t pframe_idx;
	pdir[VMEM_ADDR / PT_SIZE].val = make_pde(va_to_pa(ptable));
	for(pframe_idx = VMEM_ADDR; pframe_idx < VMEM_ADDR + SCR_SIZE + PAGE_SIZE; pframe_idx += PAGE_SIZE) {
		ptable->val = make_pte(pframe_idx);
		ptable++;
	}
	*/
	PDE *pdir = (PDE *)va_to_pa(get_updir());
	PTE *ptable = (PTE *)va_to_pa(vptable);
	
	pdir[VMEM_ADDR / PT_SIZE].val = make_pde(ptable); 
	ptable += (VMEM_ADDR >> 12);
	uint32_t pframe_addr;
	for(pframe_addr = VMEM_ADDR; pframe_addr < VMEM_ADDR + SCR_SIZE; pframe_addr += PAGE_SIZE) {
		ptable->val = make_pte(pframe_addr);
		ptable++;
	}
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
