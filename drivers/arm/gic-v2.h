#ifndef INC_DRIVERS_ARM_GIC_H
#define INC_DRIVERS_ARM_GIC_H

#include <core/types.h>

struct gic_d_regs {
	volatile u32 CTLR;             // 0x000
	volatile u32 TYPER;            // 0x004
	volatile u32 IIDR;             // 0x008
	volatile u32 _reserved0[29];   // 0x00c-07c
	volatile u32 IGROUPRn[32];     // 0x080-0fc
	volatile u32 ISENABLERn[32];   // 0x100-17c 
	volatile u32 ICENABLERn[32];   // 0x180-1fc
	volatile u32 ISPENDRn[32];     // 0x200-27c
	volatile u32 ICPENDRn[32];     // 0x280-2fc
	volatile u32 ISACTIVERn[32];   // 0x300-37c
	volatile u32 ICACTIVERn[32];   // 0x380-3fc
	volatile u32 IPRIORITYRn[255]; // 0x400-7f8
	volatile u32 _reserved3;       // 0x7fc
	volatile u32 ITARGETSRn[255];  // 0x800-bf8
	volatile u32 _reserved4;       // 0xbfc
	volatile u32 ICFGRn[64];       // 0xc00-cfc
	volatile u32 _reserved5[64];   // 0xd00-dfc
	volatile u32 NSACRn[64];       // 0xe00-efc
	volatile u32 SGIR;             // 0xf00
	volatile u32 _reserved6[3];    // 0xf04-f0c
	volatile u32 CPENDSGIRn[4];    // 0xf10-f1c
	volatile u32 SPENDSGIRn[4];    // 0xf20-f2c
};

struct gic_c_regs {
	volatile u32 CTLR;           // 0x0000
	volatile u32 PMR;            // 0x0004
	volatile u32 BPR;            // 0x0008
	volatile u32 IAR;            // 0x000c
	volatile u32 EOIR;           // 0x0010
	volatile u32 RPR;            // 0x0014
	volatile u32 HPPIR;          // 0x0018
	volatile u32 ABPR;           // 0x001c
	volatile u32 AIAR;           // 0x0020
	volatile u32 AEOIR;          // 0x0024
	volatile u32 AHPPIR;         // 0x0028
	volatile u32 _reserved0[47]; // 0x002c-cc 
	volatile u32 APRn[4];        // 0x00d0-dc
	volatile u32 NSAPRn[4];      // 0x00e0-ec
	volatile u32 _reserved1[3];  // 0x00f0-f8
	volatile u32 IIDR;           // 0x00fc
};

u32 *gic_c_DIR(struct gic_c_regs *regs);

#endif
