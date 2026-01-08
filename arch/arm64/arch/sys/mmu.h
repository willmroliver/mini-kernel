#ifndef INC_ARCH_SYS_MMU_H
#define INC_ARCH_SYS_MMU_H

#include <core/types.h>

typedef struct {
	paddr_t pte[512];
} mmu_table_t;

#endif
