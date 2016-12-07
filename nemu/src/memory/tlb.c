#include <nemu.h>
#include<stdlib.h>
#define tlb_size 64

struct Tlb{
	bool v;
	int tag;
	int page;
}tlb[tlb_size];

void init_tlb(){
	int i;
	for(i=0;i<tlb_size;i++)
		tlb[i].v=0;
};

hwaddr_t tlb_read(lnaddr_t addr){
	int high = addr & 0xfffff000;
	int i;
	for(i=0;i<tlb_size;i++){
		if((high==tlb[i].tag)&&(tlb[i].v==1)){
			return tlb[i].page;
		}
	}
	return -1;
}

void tlb_write(lnaddr_t lnaddr, hwaddr_t hwaddr){
	int tag = lnaddr&0xfffff000;
	int page = hwaddr&0xfffff000;
	int i;
	for(i=0;i<tlb_size;i++){
		if(tlb[i].v==0){
			tlb[i].v=1;
			tlb[i].tag=tag;
			tlb[i].page=page;
			return;
		}
	}
	i=rand()%tlb_size;
	tlb[i].v=1;
	tlb[i].tag=tag;
	tlb[i].page=page;
	return;
}
