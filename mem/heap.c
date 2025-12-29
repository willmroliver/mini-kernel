#include "heap_internal.h"
#include "mem.h"

struct heap kheap = {
	.addr = 0,
	.size = 0,
};

struct heap_fns kheap_fns = {
	.alloc = 0,
	.free = 0,
};

void _mem_set_heap(vaddr_t addr, size_t size)
{
	kheap.addr = addr;
	kheap.size = size;
}

void _mem_set_heap_fns(void *(*alloc)(struct heap*, size_t), 
		       void (*free)(struct heap*, void *))
{
	kheap_fns.alloc = alloc;
	kheap_fns.free = free;
}

void *_mem_heap_alloc(size_t size)
{
	return kheap_fns.alloc(&kheap, size);
}

void _mem_heap_free(void *data)
{
	kheap_fns.free(&kheap, data);
}
