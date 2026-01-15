#include <sys/mmu.h>
#include <arch/sys/gic.h>
#include <core/endian.h>
#include "gic-v2.h"

u32 __gic_ix_iar(struct gic_ix *ix)
{
	struct gic *gic = (struct gic *)ix;
	return gic->c->IAR;
}

volatile u32 *__gic_ix_eoir(struct gic_ix *ix)
{
	struct gic *gic = (struct gic *)ix;
	return &gic->c->EOIR;
}

void __gic_ix_enable(struct gic_ix *ix, u32 intid)
{
	struct gic *gic = (struct gic *)ix;

	u32 n = intid >> 5, mask = BIT(intid & BITS_32(4,0));
	gic->d->IGROUPRn[n] &= ~mask;
	gic->d->ISENABLERn[n] |= mask;

	n = intid >> 2;
	mask = range_set_bits_32(gic->d->ITARGETSRn[n], 1, 8*(intid&3), 8);
	gic->d->ITARGETSRn[n] = mask;
}

void __gic_ix_disable(struct gic_ix *ix, u32 intid)
{
	    struct gic *gic = (struct gic *)ix;

	    u32 n = intid >> 5, mask = BIT(intid & BITS_32(4,0));
	    gic->d->ISENABLERn[n] &= ~mask;
}

u32 *gic_c_DIR(struct gic_c_regs *gicc)
{
	return (u32 *)gicc + 0x1000;
}

struct gic *gic_driver_init(vaddr_t d_reg, vaddr_t c_reg)
{
	struct gic *gic = kmalloc(sizeof(struct gic));
	gic->ix.IAR = __gic_ix_iar;
	gic->ix.EOIR = __gic_ix_eoir;
	gic->ix.enable = __gic_ix_enable;
	gic->ix.disable = __gic_ix_disable;

	gic->d = (struct gic_d_regs *)d_reg;
	gic->c = (struct gic_c_regs *)c_reg;

	// Enable1 only relevant in hypervisor/monitor contexts
	gic->d->CTLR = GICD_CTLR_Enable0;

	// -- [7:4] Group Priority, [3:0] Sub Priority
	gic->c->BPR = 3;

	// -- No interrupts will masked by default
	gic->c->PMR = -1;

	if ((gic->d->TYPER & GICD_TYPER_SecurityExtn) == 0)
		gic->c->CTLR |= 1;

	return gic;
}

struct gic *gic_devicetree_init(struct fdt_node *dt)
{
	// @TODO - generalise compatibility test,
	//  support multiple compatible targets
	static const char *compatible = "arm,cortex-a15-gic";

	struct mmu_mapping mem;
	vaddr_t gicd, gicc;

	dt = dt_interrupt_root(dt);

	if (strcmp((char *)dt_prop(dt, FDT_PROP_COMPATIBLE)->data, compatible)
		|| dt->n_reg < 2
	)
		return 0;

	// GICD register
	mem.pa = dt->reg[0].addr;
	mem.va = -1;
	mem.size = dt->reg[0].size;
	mem.attr_indx = 1;
	mmu_map(&mem, 0);
	gicd = mem.va;

	// GICC register
	mem.pa = dt->reg[1].addr;
	mem.va = -1;
	mem.size = dt->reg[1].size;
	mem.attr_indx = 1;
	mmu_map(&mem, 0);
	gicc = mem.va;

	return gic_driver_init(gicd, gicc); 
}

u32 gic_devicetree_intid(struct fdt_node *node)
{
	struct fdt_prop_desc *irq = dt_prop(node, FDT_PROP_INTERRUPTS);
	if (irq == 0)
		return 0;

	u32 *vals = irq->data;
	vals[1] = reverse_endian_u32(vals[1]);

	switch (vals[0]) {
	case GIC_FDT_IRQ_TYPE_PPI:
		return vals[1] + GIC_IRQ_PPI_ID_OFFSET;
	case GIC_FDT_IRQ_TYPE_SPI:
		return vals[1] + GIC_IRQ_SPI_ID_OFFSET;
	default:
		return 0;
	}
}

