#include "arch/sys/mmu.h"
#include "list.h"
#include "mem.h"
#include <core/mem.h>

static inline void *mem_buddy_alloc(struct mem_ix *ix, size_t size)
{
	struct mem_buddy *mem = (struct mem_buddy *)ix;
	size_t width = mem->size;
	size_t tree_width = 2*width - 1;

	return 0;
}

static inline void mem_buddy_free(struct mem_ix *mem, void *data)
{
}

void mem_buddy_init(struct mem_buddy *mem)
{
	size_t n, order;

	// assert size is power of 2
	// then tree_size is 2*size - 1
	if (mem->size & (mem->size - 1))
		return;

	order = 0;
	for (n = mem->size; n != 1; n >>= 1 && order++);

	// initialize a free list for each order of mem size
	n = sizeof(struct list) * order;
	mem->lists = kmalloc(n);
	for (n -= 1; n >= 0; n--)
		list_init(mem->lists + n);

	n = mem->size / MMU_PAGE_SIZE;
	mem->xor = kzalloc(((2*n - 1) + 7) / 8);

	mem->ix.alloc = mem_buddy_alloc;
	mem->ix.free = mem_buddy_free;
}

