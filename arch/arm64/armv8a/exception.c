#include <core/types.h>


// --- ESR_EL1 Exception Class [bits 31-26] ---

static const u64 ESR1_EC_UNKNOWN     = 0x0;        // unknown exception
static const u64 ESR1_EC_SVE_SIMD_FP = 0x7 << 26;  // SVE, SIMD or FP instruction exception
static const u64 ESR1_EC_SVC         = 0x15 << 26; // SVC instruction
static const u64 ESR1_EC_MMU_EXT_LOW = 0x20 << 26; // MMU or sync-external from lower EL
static const u64 ESR1_EC_MMU_EXT     = 0x21 << 26; // MMU or sync-external from currnt EL

struct sys_regs {
	u64 x[31];
	u64 elr;
	u64 esr;
	u64 far;
};

void arch_sync_exception_handler(struct sys_regs *regs)
{
	// @TODO
}
