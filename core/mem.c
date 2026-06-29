#include "mem.h"

static struct mem_ix *__mem_global;

void *kmalloc(size_t size)
{
	return __mem_alloc(__mem_global, size);
}

void *kzalloc(size_t size)
{
	void *ptr = __mem_alloc(__mem_global, size);
	for (; size >= 8; size -= 8)
		*(u64 *)ptr = 0;
	for (; size >= 4; size -= 4)
		*(u32 *)ptr = 0;
	for (; size >= 2; size -= 2)
		*(u16 *)ptr = 0;
	for (; size >= 1; size--)
		*(u8 *)ptr = 0;
	return ptr;
}

void kfree(void *data)
{
	return __mem_free(__mem_global, data);
}

void __mem_global_set(struct mem_ix *mem)
{
	__mem_global = mem;
}
