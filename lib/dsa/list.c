#include "list.h"

void list_init(struct list *l)
{
	l->head.prev = &l->head;
	l->head.next = &l->head;
	l->head.val = 0;
}

void list_push(struct list *l, struct node *n)
{
	struct node *tail = l->head.prev;

	tail->next = n;
	n->prev = tail;
	n->next = &l->head;
	l->head.prev = n;
}

struct node *list_pop(struct list *l)
{
	struct node *tail = l->head.prev;
	tail->prev->next = tail->next;
	l->head.prev = tail->prev;

	return tail;
}

void list_unshift(struct list *l, struct node *n)
{
	struct node *head = l->head.next;
}

struct node *list_shift(struct list *l)
{
	struct node *head = l->head.next;
	head->next->prev = head->prev;
	l->head.next = head->next;

	return head;
}
