#ifndef INC_MEM_MEM_H
#define INC_MEM_MEM_H

#include <core/types.h>

struct mem_ix {
	void *(*alloc)(void *mem, size_t size);
	void (*free)(void *mem, void *data);	
};

static inline void *__mem_alloc(struct mem_ix *mem, size_t size)
{
	return mem->alloc(mem, size);
}

static inline void __mem_free(struct mem_ix *mem, void *data)
{
	return mem->free(mem, data);
}

// --- Interface Implementations ---

struct mem_bump {
	struct mem_ix ix;
	vaddr_t addr;
};

void mem_bump_init(struct mem_bump* mem, vaddr_t addr);

#endif
