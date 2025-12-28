.arch armv8-a

.section .boot, "ax"

.global _boot_main

_boot_start:
	ldr x0, =_boot_stack_top
	mov sp, x0

	ldr x0, =_pt_start 
	ldr x1, =_pt_end
	bl _zero_64

	bl _init_mmu    // minimum to jump to kernel VA -
			// one page for kernel, one for boot

	mrs x0, SCTLR_EL1
	orr x0, x0, #1
	msr SCTLR_EL1, x0 
	isb

.jump:
	ldr x16, =_boot_main
	br x16

_init_mmu:
	ldr x20, =0x1000          // granule size (4KB)
	mov x21, xzr              // set to 1 when .boot_va hit

.kernel_va:
	ldr x22, =_skernel_pa     // PA to map to
	ldr x2, =_skernel         // VA to map from

	ldr x1, =_pt_start        // table base
	msr TTBR1_EL1, x1	  // kernel page-tables 

	ldr x19, =0x80100010	  // 48-bit address spaces, 4KB granules
	msr TCR_EL1, x19

	ldr x19, =0xff		  // AttrIdx on PTEs 0 by default
	msr MAIR_EL1, x19	  // So we set slot 0 for MAIR to 0xFF (good default)
	
	b .build
.boot_va:
	mov x21, #1	          // .boot_va hit

	ldr x22, =_ram_start      // PA to map to
	mov x2, x22               // 1-1 mapping 
	add x1, x0, x20           // next base 

	msr TTBR0_EL1, x1         // boot page-tables
.build:
	mov x19, #0x4             // Base table + 3 stages
	orr x1, x1, #0x3          // Will get EORd immediately
.stage: 
	eor x0, x1, #0x3          // Last PTE becomes next table base 
	cmp x19, #1 
	bne .table
.page:
	ldr x10, =0x403	          // D_Page | Valid, nG = 0 (no ASID), AF = 1
	orr x1, x22, x10          // PTE = PA | Lower Attributes
	b .do
.table:
	add x1, x1, x20           // PTE = Next table base
.do:
	mov x3, #0x4
	sub x3, x3, x19           // stage

	stp x0, x1, [sp, #-0x10]!
	stp x2, lr, [sp, #-0x10]! 
	stp x3, x4, [sp, #-0x10]!
	bl _insert_pte
	ldp x3, x4, [sp], #0x10
	ldp x2, lr, [sp], #0x10
	ldp x0, x1, [sp], #0x10

	sub x19, x19, #1
	cbnz x19, .stage
	cbz x21, .boot_va

	ret

// x0 = table base, x1 = PTE, x2 = VA, x3 = stage 
_insert_pte:
	ldr x4, =0x1ff            // 9-bit mask
	mov x6, #0x3 
	subs x3, x6, x3 
	bge .shift
	mov x3, xzr
.shift:
	mov x5, #0x9
	mul x3, x3, x5 
	add x3, x3, #0xc          // 9*max(0, 3-stage) + 12 = shift
	lsr x2, x2, x3
	and x2, x2, x4            // table offset	
	mov x4, #0x8
	mul x2, x2, x4            // * entry width

	str x1, [x0, x2]
	ret

// x0 = from_addr, x1 = to_addr (8-byte aligned) 
_zero_64:
	mov x3, xzr
.loop:
	cmp x0, x1
	beq .done
	str x3, [x0], #0x8
	b .loop
.done:
	ret
