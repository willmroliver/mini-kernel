.arch armv8-a

.section .boot, "ax"

.global _boot_main

_boot_start:
	// @TODO - DTB doesn't seem to be passed to x0, so where is it?

	mov x25, x0 // DTB PA

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
	ldr x19, =_sstack
	mov sp, x19

	ldr x16, =_boot_main
	br x16

_init_mmu:
	ldr x20, =0x1000          // granule size (4KB)
	mov x21, xzr              // set to 1 when .boot_va hit

	ldr x19, =0x80100010      // 48-bit address spaces, 4KB granules
	msr TCR_EL1, x19

	ldr x19, =0xff            // AttrIdx on PTEs 0 by default
	msr MAIR_EL1, x19         // So we set slot 0 for MAIR to 0xFF (good default)
.kernel_va:
	ldr x22, =_skernel_pa     // PA to map to
	ldr x10, =0x401           // D_Block | Valid, nG = 0 (no ASID), AF = 1
	orr x22, x22, x10         // PA | Lower Attributes
	ldr x2, =_skernel         // VA to map from

	ldr x1, =_pt_start        // table base
	msr TTBR1_EL1, x1         // kernel page-tables 

	mov x19, #0x2             // base_table + 2 stages
	mov x3, xzr               // stage counter
	b .stage
.boot_va:
	mov x21, #1               // .boot_va hit

	ldr x2, =_ram_start       // 1-1 PA to VA mapping
	ldr x10, =0x403           // D_Page | Valid, nG = 0 (no ASID), AF = 1
	orr x22, x2, x10          // PA | Lower Attributes
	add x1, x0, x20           // next base 

	msr TTBR0_EL1, x1         // boot page-tables
	mov x19, #0x3             // base_table + 3 stages
	mov x3, xzr               // stage counter
	b .stage
.dtb_va:
	mov x21, #0x2		  // .boot_va & .dtb_va hit

	ldr x2, =_sdtb	          // reserved VA for dtb
	ldr x10, =0x401           // D_Block | Valid
	orr x22, x25, x10	  // DTB PA | Lower Attributes
	mrs x1, TTBR1_EL1	  // kernel address space
	
	mov x19, #0x2		  // base_table + 2 stages
	mov x3, xzr
.stage: 
	and x0, x1, #-0x1000      // Last PTE becomes next table base 
	cmp x3, x19               // are we at the target stage?
	bne .table
.page:
	mov x1, x22               // Load PTE configured above
	b .insert
.table:
	add x1, x1, x20           // PTE = Next table base
	ldr x10, =0x403           // D_Table | Valid
	orr x1, x1, x10           // | Attributes
.insert:
	stp x0, x1, [sp, #-0x10]!
	stp x2, x3, [sp, #-0x10]! 
	stp x4, lr, [sp, #-0x10]!
	bl _insert_pte
	ldp x4, lr, [sp], #0x10
	ldp x2, x3, [sp], #0x10
	ldp x0, x1, [sp], #0x10

	add x3, x3, #1
	cmp x3, x19
	bls .stage
	cbz x21, .boot_va
	cmp x21, #0x2
	bne .dtb_va

	ret

// x0 = table base, x1 = PTE, x2 = VA, x3 = stage 
_insert_pte:
	ldr x4, =0x1ff   // 9-bit mask
	mov x6, #0x3 
	sub x3, x6, x3   // 3 - stage
	mov x5, #0x9
	mul x3, x3, x5   // shift to get table offset
	add x3, x3, #0xc // last 12 bits = offset into page
	lsr x2, x2, x3
	and x2, x2, x4   // table rows	
	mov x4, #0x8
	mul x2, x2, x4   // table rows * entry width = offset

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
