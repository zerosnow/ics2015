#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "memory/memory.h"

#include <string.h>

#define NR_WP 32

static WP wp_list[NR_WP];
WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].args[0] ='\0';
		wp_list[i].value = 0;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = wp_list;
	free_ = NULL;
}

void add_wp(char *args){
	bool success;
	int value;
	value = expr(args,&success);
	if(false==success)
		printf("Expression is wrong\n");
	else{
		free_=head;
		while(free_->args[0]!='\0')free_=free_->next;
		strcpy(free_->args,args);
		free_->value=value;
	}
}

bool delete_wp(int n){
	if(n>=0&&n<NR_WP){
		free_ = &wp_list[n];
		if('\0'==free_->args[0])
			return false;
		while(free_->next!=NULL){
			strcpy(free_->args,free_->next->args);
			free_->value=free_->next->value;
			free_=free_->next;
		}
		return true;
	}
	else 
		return false;
}

void print_wp(){
	for(free_=head;free_->args[0]!='\0';free_=free_->next)
		printf("%d,%s:%u\n",free_->NO,free_->args,free_->value);
}

bool check_wp(){
	bool isChanged=false;
	bool success;
	uint32_t value;
	for(free_=head;free_->args[0]!='\0';free_=free_->next)
	{
		value=expr(free_->args,&success);
		if(free_->value!=value && true==success){
			free_->value=value;
			printf("breakpoint:%d    %s:%8u\n",free_->NO,free_->args,free_->value);
			isChanged=true;
		}
	}
	return isChanged;
}

	

/* TODO: Implement the functionality of watchpoint */


