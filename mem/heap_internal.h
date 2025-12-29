#ifndef INC_MEM_HEAP_INTERNAL_H
#define INC_MEM_HEAP_INTERNAL_H

#include <core/types.h>

struct heap {
	vaddr_t addr;
	size_t size;
};

struct heap_fns {
	void *(*alloc)(struct heap *h, size_t size);
	void (*free)(struct heap *h, void *data);
};

void _mem_set_heap(vaddr_t addr, size_t size);

void _mem_set_heap_fns(void *(*alloc)(struct heap*, size_t), 
		       void (*free)(struct heap*, void *));

#endif
