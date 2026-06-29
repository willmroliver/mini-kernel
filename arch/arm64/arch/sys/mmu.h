#ifndef INC_ARCH_SYS_MMU_H
#define INC_ARCH_SYS_MMU_H

#include <core/types.h>

#define MMU_PAGE_SIZE  0x1000
#define MMU_BLOCK_SIZE 0x200000

typedef struct {
	paddr_t pte[512];
} mmu_table_t;

#endif
