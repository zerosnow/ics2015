#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	char args[32];
	uint32_t value;
	struct watchpoint *next;
} WP;

void init_wp_list();
void add_wp();
bool delete_wp(int n);
void print_wp();
bool check_wp();

#endif
