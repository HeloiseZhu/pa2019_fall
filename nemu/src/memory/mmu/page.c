#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	uint32_t dir = laddr >> 22;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
	paddr_t pd_addr = (cpu.cr3.page_directory_base << 12) + 4 * dir;
	PDE pde;
	memcpy(&pde, hw_mem + pd_addr, 4);
	if(pde.present == 0)
		printf("pd_addr：%x\n",pd_addr);
	assert(pde.present == 1);
	
	paddr_t pt_addr = (pde.page_frame << 12) + 4 * page;
	PTE pte;
	memcpy(&pte, hw_mem + pt_addr, 4);
	if(pte.present == 0)
		printf("laddr：%x, pd_addr:%x, pt_addr:%x, eip:%x\n", laddr, pd_addr, pt_addr, cpu.eip);
	assert(pte.present == 1);
	paddr_t paddr = (pte.page_frame << 12) + offset;
	
	return paddr;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
