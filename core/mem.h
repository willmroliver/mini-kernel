#ifndef INC_MEM_H
#define INC_MEM_H

#include <core/types.h>
#include <mem/mem.h>

void *kmalloc(size_t size);

void kfree(void *data);

extern void *memcpy(void *dest, const void *src, size_t n);

void __mem_global_set(struct mem_ix *mem);

#endif
