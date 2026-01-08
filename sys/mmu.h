#ifndef INC_SYS_MMU_H
#define INC_SYS_MMU_H

#include <arch/sys/mmu.h>
#include <core/types.h>

struct mmu_mapping {
	paddr_t pa;
	vaddr_t va;
	u64 size;
	int attr_indx;
};

void __init_mmu(paddr_t start, paddr_t end, paddr_t next);

u64 mmu_map(mmu_table_t *base, struct mmu_mapping *m);

#endif
