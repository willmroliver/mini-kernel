#ifndef INC_ARCH_SYS_GIC_H
#define INC_ARCH_SYS_GIC_H

#include <core/types.h>

struct gic_handler {
	void (*fn)(void *arg);
	void *arg;
};

struct gic_ix {
	u32 (*IAR)(struct gic_ix *ix);
	volatile u32 *(*EOIR)(struct gic_ix *ix);
	void (*handle)(struct gic_ix *ix); // set by __arm_gic_global_set
	void (*done)(struct gic_ix *ix);    // set by __arm_gic_global_set
};

void __arm_gic_global_set(struct gic_ix *ix);

struct gic_ix *__arm_gic_global_get();

void gic_handler_set(struct gic_handler *h, u32 intid);

#endif
