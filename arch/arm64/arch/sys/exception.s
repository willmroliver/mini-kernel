.include "arch/sys/frame.s"

.text

.global __arch_init_exception_handlers

__arch_init_exception_handlers:
	ldr x1, =_svtable
	msr VBAR_EL1, x1
	ret

__sync_elx:
	SYS_FRAME_PUSH 1

	bl arch_sys_frame_init
	bl __kernel_sync_handler

	SYS_FRAME_POP 1
	eret

__stub_except:
	// @TODO
	eret

.section .vtable, "ax"

vector_table:
	b __stub_except // Synchronous from EL0
	.balign 0x80
	b __stub_except // IRQ/vIRQ from EL0
	.balign 0x80
	b __stub_except // FIQ/vFIQ from EL0
	.balign 0x80
	b __stub_except	// SError/VSError from EL0
	.balign 0x80
	b __sync_elx	// Synchronous from EL1-3
	.balign 0x80
	b __stub_except // IRQ/..
	.balign 0x80
	b __stub_except // FIQ/..
	.balign 0x80
	b __stub_except // SError/..
	.balign 0x80
