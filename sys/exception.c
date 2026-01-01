#include <core/types.h>
#include <arch/sys.h>

static vaddr_t *syscall_jump_table;

void __init_exception_handlers(void **sys_handlers)
{
	syscall_jump_table = (vaddr_t *)sys_handlers;
	arch_init_exception_handlers(syscall_jump_table);
}
