.section .boot, "ax"

#define tbase x0
#define pte x1
#define vaddr x2
#define stage x3
#define is_table x4
#define stageend x5
#define ctrl x6
#define bitmask x7
#define trow x8
#define existing x9
#define hold x10
#define paddr x11

_boot_start:
	ldr hold, =_boot_stack_top
	mov sp, hold 

	ldr x0, =_sptable_phys
	ldr x1, =_eptable_phys
	bl _zero_a16

	bl _init_mmu 
_boot_enable_mmu:
	mrs hold, SCTLR_EL1
	orr hold, hold, #1
	msr SCTLR_EL1, hold 
	isb

	// save pte (x1)
	mov x20, pte

	// zero .bss
	ldr x0, =_sbss
	ldr x1, =_ebss
	bl _zero_a16

	// x0 = DTB ptr, x1 = MMU config ptr
	ldr x0, =_sdtb
	ldr x2, =_k_offset
	ldr x3, =_sptable
	ldr x4, =_eptable
	mov x5, x20

	stp x4, x5, [sp, #-0x10]!
	stp x2, x3, [sp, #-0x10]!
	mov x1, sp

	ldr hold, =_sstack
	mov sp, hold 

	// enable IRQ exceptions
	msr DAIFCLR, #2
	ldr hold, =__boot_main
	br hold 
1:
	b 1b

_init_mmu:
	mov ctrl, xzr               // set to 1 when .boot_va hit

	ldr hold, =0x80100010       // 48-bit address spaces, 4KB granules
	msr TCR_EL1, hold

	ldr hold, =0x00ff           // AttrId0 = normal memory, 
				    //  AttrId1 = device memory (nGnRnE)
	msr MAIR_EL1, hold      
_init_kernel_va:
	ldr paddr, =_start_phys     // PA to map to
	ldr hold, =0x401            // D_Block | Valid, nG = 0 (no ASID), AF = 1
	orr paddr, paddr, hold      // PA | Lower Attributes
	ldr vaddr, =_start          // VA to map from

	ldr tbase, =_sptable_phys   // table base
	msr TTBR1_EL1, tbase        // kernel page-tables 
	add pte, tbase, #0x1000	    // next PTE Addr

	mov stageend, #0x2          // base_table + 2 stages
	mov stage, xzr              // stage counter
	b 1f 
_init_boot_va:
	mov ctrl, #1                // .boot_va hit
	mov tbase, pte		    // next free space for tbase
	add pte, tbase, #0x1000	    // next PTE Addr

	ldr vaddr, =_sboot	    // 1-1 PA to VA mapping
	ldr hold, =0x403            // D_Page | Valid, nG = 0 (no ASID), AF = 1
	orr paddr, vaddr, hold      // PA | Lower Attributes

	msr TTBR0_EL1, tbase        // boot page-tables
	mov stageend, #0x3          // base_table + 3 stages
	mov stage, xzr              // stage counter
	b 1f
_init_dtb_va:
	mov ctrl, #0x2	            // .boot_va & .dtb_va hit

	ldr vaddr, =_sdtb           // dtb placed at start of RAM by qemu virt board
	ldr hold, =0x401            // D_Block | Valid
	orr paddr, vaddr, hold      // DTB PA | Lower Attributes
	mrs tbase, TTBR0_EL1        // lower address space
	
	mov stageend, #0x2          // base_table + 2 stages
	mov stage, xzr
// LEVEL
1: 
	cmp stage, stageend         // are we at the target level?
	b.ne 3f
// PAGE 
2:
	mov pte, paddr              // Load PTE configured above
	mov is_table, xzr
	b 4f
// TABLE
3:
	ldr hold, =0x403            // D_Table | Valid
	orr pte, pte, hold          // | Attributes
	mov is_table, #1
// INSERT
4:
	str vaddr, [sp, #-0x10]! 
	stp stage, lr, [sp, #-0x10]!
	bl _insert_pte		    // tbase = next table to lookup, 
	                            //	pte = next free loc for new tbase
				    
	ldp stage, lr, [sp], #0x10
	ldr vaddr, [sp], #0x10

	add stage, stage, #1
	cmp stage, stageend 
	b.ls 1b

	cmp ctrl, #1
	b.lo _init_boot_va
	b.eq _init_dtb_va

	ret

// x0 = tbase, x1 = PTE, x2 = VA, x3 = stage, x4 = is_table
// returns: x0 = next table to lookup, x1 = next free space for new table

_insert_pte:
	ldr bitmask, =0x1ff           // 9-bit mask
	mov hold, #0x3 
	sub trow, hold, stage         // 3 - stage
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

.balign 16

// x0 = from_addr, x1 = to_addr (16-byte aligned) 
_zero_a16:
1:
	cmp x0, x1
	beq 2f 
	stp xzr, xzr, [x0], #0x10
	b 1b 
2:
	ret
