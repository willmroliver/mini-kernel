#ifndef INC_MEM_MEM_H
#define INC_MEM_MEM_H

#include <core/types.h>

void _mem_bump_alloc_init(vaddr_t addr, size_t size);

void *_mem_heap_alloc(size_t size);

void _mem_heap_free(void *data);

#endif
