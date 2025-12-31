#include "mem.h"

static struct mem_ix *__mem_global;

void *kmalloc(size_t size)
{
	return __mem_alloc(__mem_global, size);
}

void kfree(void *data)
{
	return __mem_free(__mem_global, data);
}

void __mem_global_set(struct mem_ix *mem)
{
	__mem_global = mem;
}
