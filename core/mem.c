#include "mem.h"
#include <mem/mem.h>

void *kalloc(size_t size)
{
	return _mem_heap_alloc(size);	
}

void kfree(void *data)
{
	_mem_heap_free(data);
}
