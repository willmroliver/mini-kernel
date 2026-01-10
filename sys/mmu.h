#ifndef INC_SYS_MMU_H
#define INC_SYS_MMU_H

#include <arch/sys/mmu.h>
#include <core/types.h>

struct mmu_config {
	vaddr_t k_offset;
	paddr_t pt_start;
	paddr_t pt_end;
	mmu_table_t *pt_next;	
};

struct mmu_mapping {
	paddr_t pa;
	vaddr_t va;
	u64 size;
	int attr_indx;
};

void __init_mmu(const struct mmu_config *c);

u64 mmu_map(struct mmu_mapping *m, mmu_table_t *base);

#endif
