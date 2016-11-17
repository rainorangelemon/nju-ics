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
void ddr3_read(hwaddr_t, void*);
void ddr3_write(hwaddr_t, void*,uint8_t*);

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

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
		if(((cache2[i+j].tag)==(addr>>18))&&(cache2[i+j].v=1)){  /*if data_size is not 64b and L2_size changed, here needs to be modified.*/
			return i+j;
		}
		if(cache2[i+j].v==0){
			empty_index=i+j;
		}
	}
	if(empty_index==-1){
		j = (rand() % L2_way); 
	}
	if(cache2[i+j].d==1){
		int k;
		for(k=0;k<data_size;k++)
			dram_write(((addr>>6)<<6)+k, 1,cache2[i+j].data[k]);
	}
	cache2[i+j].d=false;
	cache2[i+j].v=true;
	cache2[i+j].tag=addr>>18;
	int k;
	for(k=0;k<data_size;k++){
		cache2[i+j].data[k]=dram_read(((addr>>6)<<6)+k, 1);
	}
	return i+j;
}


uint32_t L1_read(uint32_t addr){
	uint32_t group_number = (addr >> 6)&(L1_group_number-1); /*if data_size is not 64, here needs to be modified.*/
	int i=group_number*(L1_size/(data_size*L1_group_number));
	int empty_index=-1;
	int j;
	for(j=0;j<L1_way;j++){
		if(((cache1[i+j].tag)==(addr>>13))&&(cache1[i+j].v=1)){  /*if data_size is not 64b and L1_size changed, here needs to be modified.*/
			return i+j;
		}
		if(cache1[i+j].v==0){
			empty_index=i+j;
		}
	}
	int L2_index=L2_read(addr);
	if(empty_index==-1){
		j = (rand() % L1_way); 
	}
	cache1[i+j].v=true;
	memcpy(cache1[i+j].data,cache2[L2_index].data,data_size);
	cache1[i+j].tag=addr>>13;
	return i+j;
}

void L2_write(uint32_t addr, uint32_t len, uint32_t data){
	uint32_t group_number = (addr >> 6)&(L2_group_number-1); /*if data_size is not 64, here needs to be modified.*/
        int i=group_number*(L2_size/(data_size*L2_group_number));
	int index = addr & (data_size-1);
	int j;
	for(j=0;j<L2_way;j++){
		if(((cache2[i+j].tag)==(addr>>18))&&(cache2[i+j].v=1)){  /*if data_size is not 64b and L2_size changed, here needs to be modified.*/
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
		if(((cache1[i+j].tag)==(addr>>13))&&(cache1[i+j].v=1)){  /*if data_size is not 64b and L1_size changed, here needs to be modified.*/
			memcpy(cache1[i+j].data+index,&data,len);
			break;
		}
	}
	L2_write(addr,len,data);
}

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	printf("read: addr: %x len: %u",addr,len);
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
	printf(", result: %x\n",result);
	return result;
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	printf("write: addr: %x len: %u, data: %x\n",addr,len,data);
	L1_write(addr, len, data);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	return hwaddr_read(addr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_write(addr, len, data);
}

