.macro SYS_FRAME_PUSH EL
	stp x29, x30, [sp, #-0x10]!
	stp x27, x28, [sp, #-0x10]!

	add x28, sp, #0x20 // store original sp

	stp x25, x26, [sp, #-0x10]!
	stp x23, x24, [sp, #-0x10]!
	stp x21, x22, [sp, #-0x10]!
	stp x19, x20, [sp, #-0x10]!
	stp x17, x18, [sp, #-0x10]!
	stp x15, x16, [sp, #-0x10]!
	stp x13, x14, [sp, #-0x10]!
	stp x11, x12, [sp, #-0x10]!
	stp x9, x10, [sp, #-0x10]!
	stp x7, x8, [sp, #-0x10]!
	stp x5, x6, [sp, #-0x10]!
	stp x3, x4, [sp, #-0x10]!
	stp x1, x2, [sp, #-0x10]!

	mrs x1, ELR_EL\EL
	mrs x2, ESR_EL\EL
	mrs x3, FAR_EL\EL

	stp x28, x0, [sp, #-0x10]!
	stp x2, x3, [sp, #-0x10]!
	stp xzr, x1, [sp, #-0x10]!

	// reserve some space for sys_frame_ix
	sub sp, sp, #0x40

	// x0 = regs, sp = sys_frame mem bound
	add x0, sp, #0x48
	mov x1, sp
.endm

.macro SYS_FRAME_POP EL
	add sp, sp, #0x60

	ldp x28, x0, [sp], #0x10

	ldp x1, x2, [sp], #0x10
	ldp x3, x4, [sp], #0x10
	ldp x5, x6, [sp], #0x10
	ldp x7, x8, [sp], #0x10
	ldp x9, x10, [sp], #0x10
	ldp x11, x12, [sp], #0x10
	ldp x13, x14, [sp], #0x10
	ldp x15, x16, [sp], #0x10
	ldp x17, x18, [sp], #0x10
	ldp x19, x10, [sp], #0x10
	ldp x21, x22, [sp], #0x10
	ldp x23, x24, [sp], #0x10
	ldp x25, x26, [sp], #0x10
	ldp x27, x28, [sp], #0x10
	ldp x29, x30, [sp], #0x10
.endm
