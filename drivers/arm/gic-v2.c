#include "gic.h"
 
u32 *gic_c_DIR(struct gic_c_regs *regs)
{
	return (u32 *)regs + 0x1000;
}
