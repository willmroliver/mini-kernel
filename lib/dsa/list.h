#ifndef INC_LIB_DSA_LIST_H
#define INC_LIB_DSA_LIST_H

struct node {
	struct node *prev;
	struct node *next;
	void *val;
};

struct list {
	struct node head;
};

void list_init(struct list *l);

void list_push(struct list *l, struct node *n);

struct node *list_pop(struct list *l);

void list_unshift(struct list *l, struct node *n);

struct node *list_shift(struct list *l);
	
#endif
