#include "common.h"
#include <stdlib.h>
#include "burst.h"
#include "cpu/reg.h"
#define data_size 64
#define L1_size 64*1024
#define L2_size 4*1024*1024
#define L1_way 8
#define L2_way 16
#define L1_group_number L1_size/(L1_way*data_size)
#define L2_group_number L2_size/(L2_way*data_size)
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
hwaddr_t tlb_read(uint32_t);
void tlb_write(uint32_t,uint32_t);
int is_mmio(hwaddr_t addr);
void mmio_write(hwaddr_t addr,size_t len,uint32_t data,int map_NO);
uint32_t mmio_read(hwaddr_t addr,size_t len,int map_NO);

struct Cache_L1{
	bool v;
	uint32_t tag;
	uint8_t data[data_size];
}cache1[L1_size/data_size];

struct Cache_L2{
	bool d;
	bool v;
	uint32_t tag;
	uint8_t data[data_size];
}cache2[L2_size/data_size];

void cache_initial(){
        int i;
        for(i=0;i<L1_size/(data_size);i++){
        	cache1[i].v = false;
        }       
	for(i=0;i<L2_size/(data_size);i ++){
		cache2[i].v = false;
	}
}

uint32_t L2_read(uint32_t addr){
	uint32_t group_number = (addr >> 6)&(L2_group_number-1); /*if data_size is not 64, here needs to be modified.*/
	int i=group_number*(L2_size/(data_size*L2_group_number));
	int empty_index=-1;
	int j;
	for(j=0;j<L2_way;j++){
		if(((cache2[i+j].tag)==(addr>>18))&&(cache2[i+j].v==1)){  /*if data_size is not 64b and L2_size changed, here needs to be modified.*/
			return i+j;
		}
		if(cache2[i+j].v==0){
			empty_index=i+j;
		}
	}
	if(empty_index==-1){
		j = (rand() % L2_way);
		empty_index=i+j;
	}
	if((cache2[empty_index].d==1)&&(cache2[empty_index].v==1)){
		int k;
		for(k=0;k<data_size;k++)
			dram_write(((addr>>6)<<6)+k, 1,cache2[empty_index].data[k]);
	}
	cache2[empty_index].d=false;
	cache2[empty_index].v=true;
	cache2[empty_index].tag=addr>>18;
	int k;
	for(k=0;k<data_size;k++){
		cache2[empty_index].data[k]=dram_read(((addr>>6)<<6)+k, 1);
	}
	return empty_index;
}


uint32_t L1_read(uint32_t addr){
	uint32_t group_number = (addr >> 6)&(L1_group_number-1); /*if data_size is not 64, here needs to be modified.*/
	int i=group_number*(L1_size/(data_size*L1_group_number));
	int empty_index=-1;
	int j;
	for(j=0;j<L1_way;j++){
		if(((cache1[i+j].tag)==(addr>>13))&&(cache1[i+j].v==1)){  /*if data_size is not 64b and L1_size changed, here needs to be modified.*/
			return i+j;
		}
		if(cache1[i+j].v==0){
			empty_index=i+j;
		}
	}
	int L2_index=L2_read(addr);
	if(empty_index==-1){
		j = (rand() % L1_way); 
		empty_index=i+j;
	}
	cache1[empty_index].v=true;
	memcpy(cache1[empty_index].data,cache2[L2_index].data,data_size);
	cache1[empty_index].tag=addr>>13;
	return empty_index;
}

void L2_write(uint32_t addr, uint32_t len, uint32_t data){
	uint32_t group_number = (addr >> 6)&(L2_group_number-1); /*if data_size is not 64, here needs to be modified.*/
        int i=group_number*(L2_size/(data_size*L2_group_number));
	int index = addr & (data_size-1);
	int j;
	for(j=0;j<L2_way;j++){
		if(((cache2[i+j].tag)==(addr>>18))&&(cache2[i+j].v==1)){  /*if data_size is not 64b and L2_size changed, here needs to be modified.*/
			memcpy(cache2[i+j].data+index,&data,len);
			cache2[i+j].d=true;
			return ;
		}
	}
	int L2_index=L2_read(addr);
	memcpy(cache2[L2_index].data+index,&data,len);
	cache2[L2_index].d=false;
	dram_write(addr,len,data);
	return ;
}

void L1_write(uint32_t addr, uint32_t len, uint32_t data){
        uint32_t group_number = (addr >> 6)&(L1_group_number-1); /*if data_size is not 64, here needs to be modified.*/
        int i=group_number*(L1_size/(data_size*L1_group_number));
	int index = addr & (data_size-1);
	int j;
	for(j=0;j<L1_way;j++){
		if(((cache1[i+j].tag)==(addr>>13))&&(cache1[i+j].v==1)){  /*if data_size is not 64b and L1_size changed, here needs to be modified.*/
			memcpy(cache1[i+j].data+index,&data,len);
			break;
		}
	}
	L2_write(addr,len,data);
}

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int io_NO=is_mmio(addr);
	if(io_NO==-1){
		int index = addr & (data_size-1);
		uint8_t piece[4];
		memset(piece,0,sizeof(piece));
		int L1_index = L1_read(addr);
		if(data_size<=index+len){
			memcpy(piece,cache1[L1_index].data+index,data_size-index);
			int L1_index_ = L1_read(addr+len);
			memcpy(piece+(data_size-index),cache1[L1_index_].data,len-(data_size-index));
		}else{
			memcpy(piece,cache1[L1_index].data+index,len);
		}
		/*below needs to change if correct*/
		uint32_t result;
		int zero=0;
		result=(unalign_rw(piece+zero,4)) & (~0u >> ((4 - len) << 3));
		return result;
	}else
		return mmio_read(addr,len,io_NO);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int io_NO=is_mmio(addr);
	if(io_NO!=-1){
		mmio_write(addr,len,data,io_NO);
	}else
		L1_write(addr, len, data);
}

hwaddr_t page_translate(lnaddr_t addr){
	struct PAGE_descriptor dir;
	struct PAGE_descriptor page;
	int hwaddr=tlb_read(addr);
	if(hwaddr!=-1)
		return hwaddr+(addr&0xfff);
	dir.val = hwaddr_read((cpu.cr3.page_directory_base<<12)+(addr>>22)*4,4);
	Assert(dir.p,"directory has a problem\n");
	page.val = hwaddr_read((dir.addr<<12)+((addr>>12)&0x3ff)*4,4);
	Assert(page.p,"page has a problem\n");
	hwaddr=(page.addr<<12)+(addr&0xfff);
	tlb_write(addr,hwaddr);
	return hwaddr;
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	assert(len==1||len==2||len==4);
	if((cpu.cr0.protect_enable==1)&&(cpu.cr0.paging==1)){
		if((addr&0xfffff000)!=((addr+len-1)&(0xfffff000))){
			Assert(0,"wrong lnaddr_read!\n");
		}else{
			hwaddr_t hwaddr = page_translate(addr);
			return hwaddr_read(hwaddr,len);
		}
	}
	else
		return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	assert(len==1||len==2||len==4);
	if((cpu.cr0.protect_enable==1)&&(cpu.cr0.paging==1)){
		if((addr&0xfffff000)!=((addr+len-1)&(0xfffff000))){
			Assert(0,"wrong lnaddr_write!\n");
		}else{
			hwaddr_t hwaddr = page_translate(addr);
			hwaddr_write(hwaddr,len,data);
		}
	}else{
		hwaddr_write(addr,len,data);
	}
}

void load_seg(uint8_t sreg){
	lnaddr_t offset=(lnaddr_t)cpu.gdtr.base+8*seg(sreg).selector.detail.index;
	uint32_t temp[2];
	struct SEG_descriptor *gate= (struct SEG_descriptor *)temp;
	temp[0]=lnaddr_read(offset,4);
	temp[1]=lnaddr_read(offset+4,4);
	seg(sreg).base=(gate->one).detail.base+((gate->two).detail.base2<<16)+((gate->two).detail.base3<<24);
	seg(sreg).limit=(gate->one).detail.limit+((gate->two).detail.limit2<<16);
	seg(sreg).dirty=true;
}

lnaddr_t seg_translate(swaddr_t addr,uint8_t sreg){
	if(cpu.cr0.protect_enable==1){
		if(seg(sreg).dirty==false){
			load_seg(sreg);
		}
		return addr+seg(sreg).base;
	}else{
		return (lnaddr_t) addr;
	}
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr,sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=seg_translate(addr,sreg);
	lnaddr_write(lnaddr, len, data);
}

