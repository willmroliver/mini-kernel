#include <core/types.h>
#include <arch/sys/sys.h>

static vaddr_t *syscall_jump_table;

void __init_exception_handlers(void **syscalls)
{
	syscall_jump_table = (vaddr_t *)syscalls;
	__arch_init_exception_handlers(syscall_jump_table);
}
