#ifndef INC_MEM_H
#define INC_MEM_H

#include <core/types.h>

void *kalloc(size_t n);

void kfree(void *data);

#endif
