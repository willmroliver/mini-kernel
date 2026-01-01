.arch armv8-a

.include "armv8a/macros.s"

.text

.global arch_init_exception_handlers

arch_init_exception_handlers:
	ldr x1, =_svtable
	msr VBAR_EL1, x1
	ret

__sync_elx:
	PUSH_REGS 1

	mov x0, sp
	bl arch_sync_exception_handler

	POP_REGS 1
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
