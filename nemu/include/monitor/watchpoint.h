#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	int address;
	int value;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */

} WP;

void init_wp_list();
void add_wp();
bool delete_wp(int n);
extern WP *head, *free_;

#endif
