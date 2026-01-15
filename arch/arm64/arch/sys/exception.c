#include <core/types.h>
#include <sys/sys.h>
#include "gic.h"
#include "reg.h"

// --- ESR_EL1 Exception Class [bits 31-26] ---
#define ESR1_EC_MASK (0x3f << 26)
// --- ESR_EL1 Instruction Length for synchronous exceptions [bit 25] ---
#define ESR1_IL_MASK (0x1 << 25)
// --- ESR_EL1 Instruction Specific Syndrome [bits 24-0] ---
#define ESR1_ISS_MASK (0x1ffffff)

#define ESR1_EC_UNKNOWN     (0x0)        // unknown exception
#define ESR1_EC_SVE_SIMD_FP (0x7 << 26)  // SVE, SIMD or FP instruction exception
#define ESR1_EC_SVC         (0x15 << 26) // SVC instruction
#define ESR1_EC_MMU_EXT_LOW (0x20 << 26) // MMU or sync-external from lower EL
#define ESR1_EC_MMU_EXT     (0x21 << 26) // MMU or sync-external from currnt EL

// Assumes AArch64; indicates SIMD or FP instruction executed with FPEN = 0
#define ISS_SVE_SIMD_FP_FPEN_TRAP (0x1e00000)
#define ISS_SVE_SIMD_FP_MASK      (0x1f00000)

// Attains the immediate value passed with the SVC instruction
#define ISS_SVC_IMMEDIATE_MASK (0xffff);

struct sys_frame {
	struct sys_frame_ix ix;
	u64 elr;
	u64 esr;
	u64 far;
	u64 sp;
	u64 x[31];
};

static vaddr_t __arch_sys_frame_at(struct sys_frame_ix *frame)
{
	return ((struct sys_frame *)(frame))->elr;
}

static vaddr_t __arch_sys_frame_ret(struct sys_frame_ix *frame)
{
	return ((struct sys_frame *)(frame))->x[30];
}

static vaddr_t __arch_sys_frame_stack(struct sys_frame_ix *frame)
{
	return ((struct sys_frame *)(frame))->sp;
}

static int __arch_sys_frame_exception(struct sys_frame_ix *frame)
{
	return !!((struct sys_frame *)(frame))->esr;
}

static void __sve_simd_fp_handler(struct sys_frame *frame, size_t iss);

struct sys_frame *arch_sys_frame_init(vaddr_t regs, vaddr_t sp)
{
	size_t n = sizeof(struct sys_frame_ix);
	struct sys_frame *frame = (void *)(regs - n);

	frame->ix.at = __arch_sys_frame_at;
	frame->ix.ret = __arch_sys_frame_ret;
	frame->ix.stack = __arch_sys_frame_stack;
	frame->ix.exception = __arch_sys_frame_exception;
	
	return frame;
}

void __kernel_sync_handler(struct sys_frame *frame)
{
	size_t esr = frame->esr;
	size_t ec = esr & ESR1_EC_MASK;
	size_t il = esr & ESR1_IL_MASK;
	size_t iss = esr & ESR1_ISS_MASK;

	switch (ec) {
	case ESR1_EC_UNKNOWN:
		return;
	case ESR1_EC_SVE_SIMD_FP:
		__sve_simd_fp_handler(frame, iss);
		return;
	case ESR1_EC_SVC:
		// @TODO - how should we handle SVC calls from kernel space?
		return;
	case ESR1_EC_MMU_EXT_LOW:
		// @TODO - lazy mem-mapping for user-space
		return;
	case ESR1_EC_MMU_EXT:
		// @TODO - lazy mem-mapping for kernel-space
		return;
	default:
		return;
	}
}

static void __sve_simd_fp_handler(struct sys_frame *frame, size_t iss)
{
	switch (iss & ISS_SVE_SIMD_FP_MASK) {
	case ISS_SVE_SIMD_FP_FPEN_TRAP:
		sys_reg_isb_orr_CPACR_EL1(SYS_REG_CPACR_EL1_FPEN_K);
		return;
	default:
		return;
	}
}

void __kernel_irq_handler(struct sys_frame *frame)
{
	struct gic_ix *ix = __arm_gic_global_get();
	ix->handle(ix);
	ix->done(ix);
}
