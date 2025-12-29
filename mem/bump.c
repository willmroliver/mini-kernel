#include "heap_internal.h"
#include "mem.h"

struct {
	size_t at;
} _bump = {
	.at = 0,
};

static void *_mem_bump_alloc(struct heap *h, size_t size)
{
	void *data = (void *)(h->addr + _bump.at);
	_bump.at += size;
	return data;
}

static void _mem_bump_free(struct heap *h, void *data)
{
	// do nothing - it's a bump allocator!
}

void _mem_bump_alloc_init(vaddr_t addr, size_t size)
{
	_mem_set_heap(addr, size);
	_mem_set_heap_fns(_mem_bump_alloc, _mem_bump_free);
}
