KERNEL_VMA = 0xffff000000000000 ;

PHDRS
{
  boot   PT_LOAD FLAGS(RX) ;
  vtable PT_LOAD FLAGS(RX) ;
  text   PT_LOAD FLAGS(RX) ;
  data   PT_LOAD FLAGS(RW) ;
  rodata PT_LOAD FLAGS(R)  ;
}

ENTRY(_sboot)

SECTIONS
{
  . = LOAD_ADDRESS ;

  _ram_start = . ;

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

  .tables (NOLOAD) : {
    . = ALIGN(0x1000) ; 
    _pt_start = . ;
    . += 0x10000 ;
    _pt_end = . ;
  } :NONE

  . = KERNEL_VMA ;
  _skernel = . ;
  
  __phys = ALIGN(__phys, 2M) ;
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
  
  .data ALIGN(0x4) : AT(ADDR(.text) + SIZEOF(.text)) {
    _sdata = . ;
    *(.data*) ;
    _edata = . ;
  } :data

  .rodata ALIGN(0x4) : AT(ADDR(.data) + SIZEOF(.data)) {
    _srodata = . ;
    *(.rodata*) ; 
    _erodata = . ;
  } :rodata

  .bss ALIGN(0x4) (NOLOAD) : {
    _sbss = . ;
    *(.bss*) ;
    _ebss = . ;
  } :data


  . = ALIGN(0x1000);

  .stack (NOLOAD) : {
    _estack = . ;
    . = . + 0x4000 ;
    _sstack = . - 0x10;
  } :NONE

  .heap (NOLOAD) : {
    _sheap = . ;
    . = . + 1M ;
    _eheap = . ;
  } :NONE

  . = ALIGN(2M) ;

  _end = . ;
}
