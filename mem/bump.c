#include "mem.h"

static inline void *mem_bump_alloc(void *mem, size_t size)
{
	struct mem_bump *b = mem;
	void *data = (void *)b->addr;
	b->addr += size;
	return data;
}

static inline void mem_bump_free(void *mem, void *data)
{
	// do nothing - it's a bump allocator!
}

void mem_bump_init(struct mem_bump *mem, vaddr_t addr)
{
	mem->ix.alloc = mem_bump_alloc;
	mem->ix.free = mem_bump_free;
	mem->addr = addr;
}
