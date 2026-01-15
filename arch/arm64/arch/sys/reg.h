#ifndef INC_ARCH_REG_CPACR_EL1_H
#define INC_ARCH_REG_CPACR_EL1_H

#include <core/types.h>

#define SYS_REG_CPACR_EL1_FPEN_K (0x100000) // Enable only for EL1
#define SYS_REG_CPACR_EL1_FPEN   (0x300000) // Enable FP & SIMD for EL0-1

extern void sys_reg_isb_orr_CPACR_EL1(u64 bits);

#endif
