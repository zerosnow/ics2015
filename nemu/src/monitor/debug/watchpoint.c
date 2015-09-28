#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "memory/memory.h"

#define NR_WP 32

static WP wp_list[NR_WP];
WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].address = 0;
		wp_list[i].value = 0;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = wp_list;
	free_ = NULL;
}

void add_wp(char *args){
	bool success;
	int addr;
	addr = expr(args,&success);
	if(false==success)
		printf("Expression is wrong\n");
	else{
		free_=head;
		while(free_->address!=0)free_=free_->next;
		free_->address=addr;
		free_->value=swaddr_read(addr,4);
	}
}


bool delete_wp(int n){
	if(n>=0&&n<NR_WP){
		free_ = &wp_list[n];
		if(0==free_->address)
			return false;
		while(free_->next!=NULL){
			free_->address=free_->next->address;
			free_->value=free_->next->value;
			free_=free_->next;
		}
		return true;
	}
	else 
		return false;
}

	

/* TODO: Implement the functionality of watchpoint */


