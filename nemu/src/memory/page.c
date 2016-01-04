#include <nemu.h>

#define DIR(addr) ((addr)>>22)
#define PAGE(addr) (((addr)>>12) & 0x3ff)
#define OFFSET(addr) ((addr) & 0xfff)

typedef union {
	struct {
		uint32_t P 	:1;
		uint32_t W 	:1;
		uint32_t U 	:1;
		uint32_t PWT 	:1;
		uint32_t PCD 	:1;
		uint32_t A 	:1;
		uint32_t D 	:1;
		uint32_t zero 	:2;
		uint32_t AVL 	:3;
		uint32_t page_base 	:20;
	};
	uint32_t val;
}PAGE_DES;

hwaddr_t page_translate(lnaddr_t addr) {
	PAGE_DES dir;
	PAGE_DES page;
	if (cpu.cr0.paging == 0)return addr;

	dir.val = hwaddr_read((cpu.cr3.page_directory_base<<12)+DIR(addr), 4);
	printf("%x, %x\n", cpu.cr3.page_directory_base, dir.val);
	Assert(dir.P, "dir do not exist");
	page.val = hwaddr_read((dir.page_base<<12)+PAGE(addr), 4);
	Assert(page.P, "page do not exist");
	return (page.page_base<<12)+OFFSET(addr);
}