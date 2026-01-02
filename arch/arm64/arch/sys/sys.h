#ifndef INC_ARCH_SYS_H
#define INC_ARCH_SYS_H

#include <core/types.h>

extern void __arch_init_exception_handlers(vaddr_t *syscall_jump_table);

#endif

