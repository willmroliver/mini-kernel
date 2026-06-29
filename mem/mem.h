#ifndef INC_MEM_MEM_H
#define INC_MEM_MEM_H

#include <core/types.h>
#include <lib/dsa/ring.h>

struct mem_ix {
	void *(*alloc)(struct mem_ix *ix, size_t size);
	void (*free)(struct mem_ix *ix, void *data);	
};

static inline void *__mem_alloc(struct mem_ix *ix, size_t size)
{
	return ix->alloc(ix, size);
}

static inline void __mem_free(struct mem_ix *ix, void *data)
{
	return ix->free(ix, data);
}

// --- Interface Implementations ---

struct mem_bump {
	struct mem_ix ix;
	addr_t addr;
};

void mem_bump_init(struct mem_bump* ix);

struct mem_buddy {
	struct mem_ix ix;
	addr_t addr;
	size_t size;
	struct list *lists;
	u8     *xor;
};

void mem_buddy_init(struct mem_buddy *ix);

#endif
