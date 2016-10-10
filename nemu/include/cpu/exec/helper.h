#ifndef __EXEC_HELPER_H__
#define __EXEC_HELPER_H__

#include "cpu/helper.h"
#include "cpu/decode/decode.h"

#define make_helper_v(name) \
	make_helper(concat(name, _v)) { \
		return (ops_decoded.is_operand_size_16 ? concat(name, _w) : concat(name, _l)) (eip); \
	}

#define do_execute concat4(do_, instr, _, SUFFIX)

#define make_instr_helper(type) \
	make_helper(concat5(instr, _, type, _, SUFFIX)) { \
		return idex(eip, concat4(decode_, type, _, SUFFIX), do_execute); \
	}

extern char assembly[];
#ifdef DEBUG
#define print_asm(...) Assert(snprintf(assembly, 80, __VA_ARGS__) < 80, "buffer overflow!")
#else
#define print_asm(...)
#endif

#define print_asm_template1() \
	print_asm(str(instr) str(SUFFIX) " %s", op_src->str)

#define print_asm_template2() \
	print_asm(str(instr) str(SUFFIX) " %s,%s", op_src->str, op_dest->str)

#define print_asm_template3() \
	print_asm(str(instr) str(SUFFIX) " %s,%s,%s", op_src->str, op_src2->str, op_dest->str)

#define set_of(doctor,patient,DATA_BYTE,cin) \
	if(DATA_BYTE==1){ \
	        unsigned int result1=doctor+patient+cin; \
		unsigned int result2=(result1&0xff)>>7; \
		unsigned int result3=((result1)<<23)>>23; \
		cpu.flags.bits.of=result2^result3; \
	}else if(DATA_BYTE==2){ \
		unsigned int result1=doctor+patient+cin; \
		unsigned int result2=((result1&0xffff))>>15; \
                unsigned int result3=((result1)<<15)>>15; \
                cpu.flags.bits.of=result2^result3; \
	}else if(DATA_BYTE==4){ \
		unsigned long long doctor2=doctor; \
                unsigned long long patient2=patient; \
                unsigned long long result4=doctor2+patient2+cin; \
                unsigned long long result2=((result4)&(0x0000ffff))>>31; \
                unsigned long long result3=(((result4)&(0xffff0000))<<31)>>31; \
                cpu.flags.bits.of=result2^result3; \
	}

#define set_sf(patient,DATA_BYTE) \
	unsigned int result5; \
	if(DATA_BYTE==1){ \
		result5=patient&0xff; \
		result5=result5>>7; \
	}else if(DATA_BYTE==2){ \
		result5=patient&0xffff; \
		result5=result5>>15; \
	}else if(DATA_BYTE==4){ \
	        result5=patient&0xffffffff; \
		result5=result5>>31; \
	} \
	cpu.flags.bits.sf=result5;

#define set_zf(patient)\
	        if(patient==0){ \
	                cpu.flags.bits.zf=1; \
	        }else{ \
	                cpu.flags.bits.zf=0; \
	        }

#define set_pf(patient)\
	        unsigned int result12=(patient)&(0xf); \
		unsigned int result13=((patient)&(0xf0))>>4; \
	        if((result12==0)||(result12==3)||(result12==5)||(result12==6)||(result12==9)||(result12==10)||(result12==12)||(result12==15)) \
	                cpu.flags.bits.pf=1; \
	        else \
	                cpu.flags.bits.pf=0; \
	        if((result13==0)||(result13==3)||(result13==5)||(result13==6)||(result13==9)||(result13==10)||(result13==12)||(result13==15)) \
	                cpu.flags.bits.pf=(cpu.flags.bits.pf); \
	        else \
	                cpu.flags.bits.pf=~(cpu.flags.bits.pf);

#define set_cf(doctor,patient,cin,DATA_BYTE) \
        if(DATA_BYTE==1){ \
        	unsigned int result6=doctor+patient+cin; \
		unsigned int result7=((result6)<<23)>>31; \
                cpu.flags.bits.cf=result7^cin; \
        }else if(DATA_BYTE==2){ \
                unsigned int result6=doctor+patient+cin;\
		unsigned int result7=((result6)<<15)>>31; \
                cpu.flags.bits.cf=result7^cin; \
        }else if(DATA_BYTE==4){ \
                unsigned long long doctor1=(uint32_t)doctor; \
		unsigned long long patient1=(uint32_t)patient; \
		unsigned long long result8=doctor1+patient1; \
                result8=result8+cin; \
		unsigned long long result7=((result8)<<31)>>63; \
                cpu.flags.bits.cf=result7^cin; \
        }

#define set_af(doctor,patient,cin) \
		int doctor3=(doctor)&0xf; \
		int patient3=(patient)&0xf; \
		int result9=doctor3+patient3+cin; \
		int result10=result9>>4; \
		int result11=(result9&0xf)>>3; \
		cpu.flags.bits.af=result10^result11;

#endif
