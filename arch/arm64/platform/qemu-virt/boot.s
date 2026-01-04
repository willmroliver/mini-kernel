.section .boot, "ax"

#define tbase x0
#define pte x1
#define vaddr x2
#define lvl x3
#define is_table x4
#define lvlend x5
#define ctrl x6
#define bitmask x7
#define trow x8
#define existing x9
#define hold x10
#define paddr x11

_boot_start:
	ldr hold, =_boot_stack_top
	mov sp, hold 

	ldr x0, =_pt_start 
	ldr x1, =_pt_end
	bl _zero_64

	bl _init_mmu 

	mrs hold, SCTLR_EL1
	orr hold, hold, #1
	msr SCTLR_EL1, hold 
	isb

	ldr hold, =_sstack
	mov sp, hold 

	ldr x0, =_sdtb
	ldr hold, =__boot_main
	br hold 

_init_mmu:
	mov ctrl, xzr           // set to 1 when .boot_va hit

	ldr hold, =0x80100010   // 48-bit address spaces, 4KB granules
	msr TCR_EL1, hold

	ldr hold, =0xff         // AttrIdx on PTEs 0 by default
	msr MAIR_EL1, hold      // So we set slot 0 for MAIR to 0xFF (good default)
_init_kernel_va:
	ldr paddr, =_skernel_pa // PA to map to
	ldr hold, =0x401        // D_Block | Valid, nG = 0 (no ASID), AF = 1
	orr paddr, paddr, hold  // PA | Lower Attributes
	ldr vaddr, =_skernel    // VA to map from

	ldr tbase, =_pt_start   // table base
	msr TTBR1_EL1, tbase    // kernel page-tables 
	add pte, tbase, #0x1000	// next PTE Addr

	mov lvlend, #0x2        // base_table + 2 lvls
	mov lvl, xzr            // lvl counter
	b 1f 
_init_boot_va:
	mov ctrl, #1            // .boot_va hit
	mov tbase, pte		// next free space for tbase
	add pte, tbase, #0x1000	// next PTE Addr

	ldr vaddr, =_sboot	// 1-1 PA to VA mapping
	ldr hold, =0x403        // D_Page | Valid, nG = 0 (no ASID), AF = 1
	orr paddr, vaddr, hold  // PA | Lower Attributes

	msr TTBR0_EL1, tbase    // boot page-tables
	mov lvlend, #0x3        // base_table + 3 lvls
	mov lvl, xzr            // lvl counter
	b 1f
_init_dtb_va:
	mov ctrl, #0x2	        // .boot_va & .dtb_va hit

	ldr vaddr, =_sdtb       // dtb placed at strt of RAM by qemu virt board
	ldr hold, =0x401        // D_Block | Valid
	orr paddr, vaddr, hold  // DTB PA | Lower Attributes
	mrs tbase, TTBR0_EL1    // kernel address space
	
	mov lvlend, #0x2        // base_table + 2 lvls
	mov lvl, xzr
// LEVEL
1: 
	cmp lvl, lvlend         // are we at the target level?
	b.ne 3f
// PAGE 
2:
	mov pte, paddr          // Load PTE configured above
	mov is_table, xzr
	b 4f
// TABLE
3:
	ldr hold, =0x403        // D_Table | Valid
	orr pte, pte, hold      // | Attributes
	mov is_table, #1
// INSERT
4:
	str vaddr, [sp, #-0x10]! 
	stp lvl, lr, [sp, #-0x10]!
	bl _insert_pte
	// tbase = next table to lookup, pte = next free loc for new tbase
	ldp lvl, lr, [sp], #0x10
	ldr vaddr, [sp], #0x10

	add lvl, lvl, #1
	cmp lvl, lvlend 
	b.ls 1b

	cmp ctrl, #1
	b.lo _init_boot_va
	b.eq _init_dtb_va

	ret

// x0 = tbase, x1 = PTE, x2 = VA, x3 = lvl, x4 = is_table
// returns: x0 = next table to lookup, x1 = next free space for new table

_insert_pte:
	ldr bitmask, =0x1ff           // 9-bit mask
	mov hold, #0x3 
	sub trow, hold, lvl           // 3 - lvl
	mov hold, #0x9
	mul trow, trow, hold          // shift to get table offset
	add trow, trow, #0xc          // last 12 bits = offset into page
	lsr trow, vaddr, trow
	and trow, trow, bitmask       // table rows	
	mov hold, #0x8
	mul trow, trow, hold          // table rows * entry width = offset

	ldr existing, [tbase, trow]   // test if existing entry
	cbz existing, 1f

	and tbase, existing, #-0x1000 // return existing tbase
	and pte, pte, #-0x1000        // pte = next free table base
	ret
1:
	str pte, [tbase, trow]
	cbz is_table, 2f
	and tbase, pte, #-0x1000      // return tbase addr just created
2:
	add pte, tbase, #0x1000       // increment to get next free loc
	ret

// x0 = from_addr, x1 = to_addr (8-byte aligned) 
_zero_64:
	mov x3, xzr
1:
	cmp x0, x1
	beq 2f 
	str x3, [x0], #0x8
	b 1b 
2:
	ret
