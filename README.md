# mini-kernel

`mini-kernel` is an OS kernel that aims for simplicity, clear interface boundaries 
and extensibility.

## Purpose 

Currently built for arm64, but with the hope of introducing RISC-V & x86, the 
code structure and boot strategy aim to move from architecture-specific to generic 
with as few abstraction layers as possible.

Like any good kernel, the primary focuses will be:

1) Memory Management
2) Process / Context Handling
3) FS / Persistence

## Progress

### armv8-a

Successfully initialized the exception vector table, and using it have added: 
- Dynamic enabling of FP/SIMD if an instruction which relies on these features is 
executed.
- A basic event-loop handling IRQs triggered from the on-board UART + Generic 
Interrupt Controller (v2). This will be our initial IO loop with the kernel.

Implemented basic support for dynamic configuration of page-tables in kernel VA 
(MM foundations - see `mmu_map() in /arch/arm64/arch/sys/mmu.c`).

### Boot

Focused on a bare-metal boot of the arm-based qemu 'virt' board (see 
`/arch/arm64/platform/qemu-virt/boot.c`), building out the structures and 
interfaces which will separate generic code from hardware-specifics.

In `/mem` and `/sys` we have some initial ideas of the 'contract' `/arch` impl.s 
will have to fulfil, and a point of access to configure synchronous exceptions 
(syscalls).

Similarly, some `/core` header functions expect optimised `/arch` implementations.

### Devicetree

Will later add UEFI support, but initially focusing on booting from a DTB. DTBs 
are simpler and permit focusing on drivers, driver-registration and hardware 
introspection more generally.

