#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../../lib-common/x86-inc/cpu.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { R_ES, R_CS, R_SS, R_DS, R_FS, R_GS };


/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

struct SEG_REG{
	union{
		struct{
			unsigned int dpl:2;
			unsigned int ti:1;
			unsigned int index:13;
		}detail;
		uint16_t val;
	}selector;
	uint32_t base;
	uint32_t limit;
	struct{
		uint32_t type:5;
		uint32_t dpl:2;
		uint32_t p:1;
		uint32_t avl:1;
		uint32_t zero:1;
		uint32_t b:1;
		uint32_t g:1;
	}attribute;
	bool dirty;
};


typedef struct {
    union{	
	union {
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
	} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */
        struct{
	    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
	};
    };

    swaddr_t eip;
    //EFLAGS

    	union{
		uint32_t eflags;
		struct{
			uint8_t cf:1;
			uint8_t space1:1;
			uint8_t pf:1;
			uint8_t space2:1;
			uint8_t af:1;
			uint8_t space3:1;
			uint8_t zf:1;
			uint8_t sf:1;
			uint8_t tf:1;
			uint8_t ief:1;
			uint8_t df:1;
			uint8_t of:1;
			uint8_t io:2;
			uint8_t nt:1;
			uint8_t space4:1;
			uint8_t rf:1;
			uint8_t vm:1;
			uint16_t space5;
		}bits;
	}flags;

	/*pa3 added*/
	struct {
		uint32_t base;
		uint32_t limit;
	}gdtr;

	union{
		struct SEG_REG sr[6];
		struct{
			struct SEG_REG es,cs,ss,ds,fs,gs;
		}regs;
	}seg_reg;

	CR0 cr0;
	CR3 cr3;


} CPU_state;

union SELECTOR{
	struct{
		uint16_t rpl:2;
		uint16_t ti:1;
		uint16_t index:13;
	};
	uint16_t data;
};

struct SEG_descriptor{
	union{
		struct{
			uint32_t limit:16;
			uint32_t base:16;
		}detail;
		uint32_t data;
	}one;
	union{
		struct{
			uint32_t base2:8;
			uint32_t type:5;
			uint32_t dpl:2;
			uint32_t p:1;
			uint32_t limit2:4;
			uint32_t avl:1;
			uint32_t zero:1;
			uint32_t b:1;
			uint32_t g:1;
			uint32_t base3:8;
		}detail;
		uint32_t data;
	}two;
};

struct PAGE_descriptor{
	union{
		struct{
			uint32_t p:1;
			uint32_t rw:1;
			uint32_t us:1;
			uint32_t  :2;
	  		uint32_t a :1;
			uint32_t d:1;
			uint32_t  :2;
			uint32_t avail:3;
			uint32_t addr:20;
		}
		uint32_t val;
	}
};

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])
#define seg(index) (cpu.seg_reg.sr[check_reg_index(index)])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
