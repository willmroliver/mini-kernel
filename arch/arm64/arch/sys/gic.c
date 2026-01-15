#include <core/util/bits.h>
#include "gic.h"

static const u32 IAR_MASK = BITS(12,0);
static const u32 INTID_MASK = BITS(9,0);
static const u32 CPUID_MASK = BITS(12,10);

static struct gic_handler gic_jmp[1020] = { 0 };

static struct gic_ix *__gic_global;

static u32 iar;
static u32 intid;
static u32 cpuid;

static inline void __gic_ix_handle(struct gic_ix *ix)
{
	struct gic_handler *h;
	iar = ix->IAR(ix);

	intid = iar & INTID_MASK;
	cpuid = iar & CPUID_MASK;

	h = gic_jmp + intid;
	h->fn(h->arg);
}

static inline void __gic_ix_done(struct gic_ix *ix)
{
	*(ix->EOIR(ix)) = iar;
}

void __arm_gic_global_set(struct gic_ix *ix)
{
	__gic_global = ix;
	__gic_global->handle = __gic_ix_handle;
	__gic_global->done = __gic_ix_done;
}

struct gic_ix *__arm_gic_global_get()
{
	return __gic_global;
}

void gic_handler_set(struct gic_handler *h, u32 intid)
{
	gic_jmp[intid] = *h;
}


