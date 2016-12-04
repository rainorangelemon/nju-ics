#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];
	uint8_t page_buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);

	/* Load each program segment */
	//panic("please implement me");
	int j=0;
	for(;j<elf->e_phnum;j++) {
		ph=(void*)(elf->e_phoff+j*elf->e_phentsize);
		/* Scan the program header table, load each segment into memory */
		if(ph->p_type == PT_LOAD) {

			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			int va;
			int dirty=0;
			for(va=ph->p_vaddr;va<ph->p_vaddr+ph->p_memsz;va=va+4096){
				unsigned int off = va&0xfff;
				va=va&0xfffff000;
				unsigned int addr = mm_malloc(va,4096);
				memset(page_buf,0,4096);
				unsigned int valid_num = 4096-off;
				if((ph->p_filesz-dirty)<valid_num)
					valid_num=ph->p_filesz-dirty;
				ramdisk_read((void*)(page_buf+off),ph->p_offset+ELF_OFFSET_IN_DISK+dirty,valid_num);
				dirty=dirty+valid_num;
				memcpy((void*)addr,page_buf,4096);
			}
#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t cur_brk, max_brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(cur_brk < new_brk) { max_brk = cur_brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
