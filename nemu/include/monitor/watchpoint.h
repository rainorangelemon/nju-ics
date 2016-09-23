#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	char * expr_str;
	int data;

	/* TODO: Add more members if necessary */


} WP;

WP * new_wp();
void free_wp(WP *);
WP * get_head();

#endif
