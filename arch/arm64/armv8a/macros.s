.arch armv8-a

.macro PUSH_REGS EL
	stp x0, x1, [sp, #-0x10]!
	stp x2, x3, [sp, #-0x10]!
	stp x4, x5, [sp, #-0x10]!
	stp x6, x7, [sp, #-0x10]!
	stp x8, x9, [sp, #-0x10]!
	stp x10, x11, [sp, #-0x10]!
	stp x12, x13, [sp, #-0x10]!
	stp x14, x15, [sp, #-0x10]!
	stp x16, x17, [sp, #-0x10]!
	stp x18, x19, [sp, #-0x10]!
	stp x20, x21, [sp, #-0x10]!
	stp x22, x23, [sp, #-0x10]!
	stp x24, x25, [sp, #-0x10]!
	stp x26, x27, [sp, #-0x10]!
	stp x28, x29, [sp, #-0x10]!

	mrs x1, ELR_EL\EL
	mrs x2, ESR_EL\EL
	mrs x3, FAR_EL\EL

	stp x30, x1, [sp, #-0x10]!
	stp x2, x3, [sp, #-0x10]!
.endm

.macro POP_REGS EL
	ldp x2, x3, [sp], #0x10
	ldp x1, x30, [sp], #0x10

	msr FAR_EL\EL, x3
	msr ESR_EL\EL, x2
	msr ELR_EL\EL, x1
		
	ldp x28, x29, [sp], #0x10
	ldp x26, x27, [sp], #0x10
	ldp x24, x25, [sp], #0x10
	ldp x22, x23, [sp], #0x10
	ldp x20, x21, [sp], #0x10
	ldp x18, x19, [sp], #0x10
	ldp x16, x17, [sp], #0x10
	ldp x14, x15, [sp], #0x10
	ldp x12, x13, [sp], #0x10
	ldp x10, x11, [sp], #0x10
	ldp x8, x9, [sp], #0x10
	ldp x6, x7, [sp], #0x10
	ldp x4, x5, [sp], #0x10
	ldp x2, x3, [sp], #0x10
	ldp x0, x1, [sp], #0x10
.endm
