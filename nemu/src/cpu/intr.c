#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void load_sreg(uint8_t sreg);

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	//printf("Please implement raise_intr()");
	//assert(0);
	
	//Triggrt an exception/interrupt with 'intr_no'
	//'intr_no' is the index to the IDT
	
	//Push EFLAGS, CS and EIP
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, 4, cpu.eflags.val);
	
	cpu.esp -= 4;
	uint32_t data = cpu.cs.val;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, 4, data);
	/*uint32_t data = (cpu.cs.base << 16) + cpu.cs.val;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, 4, data);
	cpu.esp -= 4;
	data = (cpu.cs.limit << 16) + (cpu.cs.base >> 16);
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, 4, data);
	cpu.esp -= 4;
	data = (cpu.cs.soft_use << 23) + (cpu.cs.privilege_level << 21) + (cpu.cs.type << 16) + (cpu.cs.limit >> 16);
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, 4, data);*/
	
	cpu.esp -= 4;
	vaddr_write((vaddr_t)cpu.esp, SREG_SS, 4, cpu.eip);
	
	//Find the IDT entry using 'intr_no'
	GateDesc gd;
	laddr_t gd_laddr = cpu.idtr.base + intr_no * 8;
	paddr_t gd_paddr = page_translate(gd_laddr);
	memcpy(&gd, hw_mem + gd_paddr, 8);
	cpu.segReg[1].val = gd.selector;
	//load_sreg(1);
	
	//Clear IF if it is an interrupt
	if(gd.type == 0xe) { //1110B
		cpu.eflags.IF = 0;
	}
	
	//Set EIF to the entry of the interrupt
	cpu.eip = (gd.offset_31_16 << 16) + gd.offset_15_0;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
