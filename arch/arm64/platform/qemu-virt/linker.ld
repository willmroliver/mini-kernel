KERNEL_VMA = 0xffff000000000000 ;

PHDRS
{
  boot       PT_LOAD FLAGS(RX) ;
  vtable     PT_LOAD FLAGS(RX) ;
  text       PT_LOAD FLAGS(RX) ;
  data       PT_LOAD FLAGS(RW) ;
  rodata     PT_LOAD FLAGS(R)  ;
}

ENTRY(_sboot)

SECTIONS
{
  . = LOAD_ADDRESS ;

  .dtb (NOLOAD) : AT(LOAD_ADDRESS) {
    _sdtb = . ;
    . += 2M ;
    _edtb = . ;
  } :NONE

  . += LOAD_OFFSET;
  __phys = . ;

  .boot : AT(__phys) {
    _sboot = . ;
    *(.boot*) ;
    . = ALIGN(0x10) ;
    _boot_stack_end = . ;
    . += 0x100 ;
    _boot_stack_top = . ;
    . = ALIGN(0x1000) ;
    _eboot = . ;
  } :boot

  __phys += SIZEOF(.boot) ;

  .ptable (NOLOAD) : AT(__phys) {
    . = ALIGN(0x1000) ; 
    _pt_start = . ;
    . += 1M ;
    _pt_end = . ;
  } :NONE

  . = KERNEL_VMA ;
  _skernel = . ;
  
  __phys = ALIGN(__phys + SIZEOF(.ptable), 2M) ;
  _skernel_pa = __phys ;

  .vtable : AT(__phys) {
    _svtable = . ;
    KEEP(*(.vtable*)) ;
    . = ALIGN(0x1000) ;
    _evtable = . ;
  } :vtable

  __phys += SIZEOF(.vtable) ;
  
  .text ALIGN(0x1000) : AT(__phys) {
    _stext = . ;
    *(.text*) ;
    _etext = . ;
  } :text

  __phys += SIZEOF(.text) ;
  
  .data ALIGN(0x4) : AT(__phys) {
    _sdata = . ;
    *(.data*) ;
    _edata = . ;
  } :data

  __phys += SIZEOF(.data) ;

  .rodata ALIGN(0x4) : AT(__phys) {
    _srodata = . ;
    *(.rodata*) ; 
    . = ALIGN(0x8) ;
    QUAD(0) ;
    _erodata = . ;
  } :rodata

  __phys += SIZEOF(.rodata) ;

  .bss ALIGN(0x4) (NOLOAD) : AT(__phys) {
    . = ALIGN(0x10) ;
    _sbss = . ;
    *(.bss*) ;
    . = ALIGN(0x10) ;
    _ebss = . ;
  } :data

  __phys += SIZEOF(.bss) ;

  . = ALIGN(0x1000);

  .stack (NOLOAD) : AT(__phys) {
    _estack = . ;
    . = . + 0x4000 ;
    _sstack = . - 0x10;
  } :NONE

  __phys += SIZEOF(.stack) ;

  .heap (NOLOAD) : AT(__phys) {
    _sheap = . ;
    . = . + 1M ;
    _eheap = . ;
  } :NONE

  . = ALIGN(2M) ;

  _end = . ;
}
