#ifndef INC_MEM_MEM_H
#define INC_MEM_MEM_H

#include <core/types.h>

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
	vaddr_t addr;
};

void mem_bump_init(struct mem_bump* ix, vaddr_t addr);

#endif
