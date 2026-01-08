#include <core/types.h>
#include <core/util/bits.h>
#include <sys/mmu.h>
#include "mmu.h"

const u64 MMU_TABLE_UATTR_NSTable   = BIT(63);
const u64 MMU_TABLE_UATTR_APTable   = BITS_64(62,61);
                                                           
const u64 MMU_TABLE_NLTA_4KB        = BITS_64(47,12);
const u64 MMU_TABLE_NLTA_16KB       = BITS_64(47,14);
const u64 MMU_TABLE_NLTA_64KB       = BITS_64(47,16);
const u64 MMU_TABLE_NLTA_MASK       = BITS_64(47,12);

const u64 MMU_TABLE_AF              = BIT(10);
const u64 MMU_TABLE_AF_MASK         = BIT(10);
const u64 MMU_TABLE_DESCRIPTOR      = BIT(1);
const u64 MMU_TABLE_VALID           = BIT(0);

const u64 MMU_PAGE_UATTR_Contiguous = BIT(52);
const u64 MMU_PAGE_UATTR_GP         = BIT(50);
const u64 MMU_PAGE_L1_BLOCK_OA_4KB  = BITS_64(47,30);
const u64 MMU_PAGE_L2_BLOCK_OA_4KB  = BITS_64(47,21);
const u64 MMU_PAGE_L2_BLOCK_OA_16KB = BITS_64(47,25);
const u64 MMU_PAGE_L2_BLOCK_OA_64KB = BITS_64(47,29);
const u64 MMU_PAGE_OA_4KB           = BITS_64(47,12);
const u64 MMU_PAGE_OA_16KB          = BITS_64(47,14);
const u64 MMU_PAGE_OA_64KB          = BITS_64(47,16);
const u64 MMU_PAGE_AF               = BIT(10);
const u64 MMU_PAGE_SH               = BITS_64(9,8);
const u64 MMU_PAGE_AttrIndx         = BITS_64(4,2);
const u64 MMU_PAGE_DESCRIPTOR       = BIT(1); 
const u64 MMU_PAGE_VALID            = BIT(0);

const u64 SIZE_PAGE                 = 0x1000;
const u64 SIZE_BLOCK                = 0x200000;

#define MMU_TABLE_KERNEL ((mmu_table_t *)(pt_start))

static paddr_t pt_start, pt_end, pt_next;

void __init_mmu(paddr_t start, paddr_t end, paddr_t next)
{	
	// at first we'll just bump-allocate -
	//    initially all page tables will be long-lived
	//    (i.e. kernel, device mmio)
	pt_start = start;
	pt_end = end;
	pt_next = next;
}

u64 mmu_map(mmu_table_t *base, struct mmu_mapping *m);

static paddr_t mmu_table_alloc(mmu_table_t *base)
{
	paddr_t pt = 0;

	// @TODO - if pt_next = _pt_end, or base != kernel,
	//    provide new allocator
	if (base == MMU_TABLE_KERNEL
		&& pt_next < (paddr_t)pt_end
	)
		pt = pt_next++;

	return pt;
}

static inline int pte_row(vaddr_t va, int stage)
{
	static const unsigned mask = BITS(8,0);
	return (va >> (((3 - stage) * 9) + 12)) & mask;
}

static inline u64 pte_page(paddr_t pa, struct mmu_mapping *m)
{
	static const u64 page_attr = 
		MMU_PAGE_VALID |
		MMU_PAGE_DESCRIPTOR |
		MMU_PAGE_AF;

	return mask_set_bits_64(pa|page_attr, m->attr_indx, ~MMU_PAGE_AttrIndx, 8);
}

static inline u64 pte_block(paddr_t pa, struct mmu_mapping *m)
{
	static const u64 block_attr =
		MMU_PAGE_VALID |
		MMU_PAGE_AF;

	return mask_set_bits_64(pa|block_attr, m->attr_indx, ~MMU_PAGE_AttrIndx, 8);
}

static inline paddr_t pte_get(mmu_table_t *table, vaddr_t va, int stage)
{
	return table->pte[pte_row(va, stage)] & MMU_TABLE_NLTA_4KB;
}

static inline void pte_insert(mmu_table_t *base, u64 pte, vaddr_t va, int stage)
{
	static const u64 table_attr = 
		MMU_TABLE_VALID |
		MMU_TABLE_DESCRIPTOR |
		MMU_TABLE_AF;

	mmu_table_t *table = base;
	paddr_t next;
	int i, r;

	for (i = 0; i < stage; i++) {
		r = pte_row(va, i);
		next = table->pte[pte_row(va, stage)];

		if (!next) {
			next = mmu_table_alloc(base) | table_attr;
			table->pte[r] = next;
		}

		table = (mmu_table_t *)(next & MMU_TABLE_NLTA_4KB);
	}

	table->pte[pte_row(va, stage)] = pte; 
}

u64 mmu_map(mmu_table_t *base, struct mmu_mapping *m)
{
	// @TODO - this is an incredibly naive implementation:
	//    - need to consider how to track pages associated to bases
	//    - storing metadata somewhere / associating to procs
	//    - releasing pages / reclaiming dead table space 
	//
	// `struct mmu_mapping` might be a useful focal point
	//    for tracking & storing
	u64 pte, pa, va, mapped = 0;
	int stage;

	// enforces page-alignment, returns actual size
	m->pa = ALIGN_FWD(m->pa, SIZE_PAGE);
	m->va = ALIGN_FWD(m->va, SIZE_PAGE);
	m->size = ALIGN_FWD(m->size, SIZE_PAGE);

	while (mapped < m->size - SIZE_BLOCK) {
		pte = pte_block(m->pa + mapped, m);
		pte_insert(base, pte, m->va + mapped, 2);
		mapped += SIZE_BLOCK;
	}

	while (mapped < m->size) {
		pte = pte_page(m->pa + mapped, m);
		pte_insert(base, pte, m->va + mapped, 2);
		mapped += SIZE_PAGE;
	}

	return m->size;
}
