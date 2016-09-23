#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP * new_wp (){
        WP * wp;
        wp = free_;
        free_=(*free_).next;
        (*wp).next=head;
        head=wp;
	return head;
}

void free_wp(WP * wp){
        if(wp==head){
 		(*head).next=free_;
                free_=head;
		head=NULL;							        }else{
	 	WP * temp;
		temp=wp;
		while((*temp).next!=wp){
			temp=(*temp).next;
		}
		(*temp).next=(*wp).next;
		(*wp).next=free_;
		free_=wp;
	}
}

WP * get_head(){
	return head;
}
