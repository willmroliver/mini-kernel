KERNEL_VA = 0xffff000000000000 ;

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
  _ram_start = LOAD_ADDRESS ;
  . = _ram_start ;

  .dtb (NOLOAD) : AT(LOAD_ADDRESS) {
    _sdtb = . ;
    . += 2M ;
    _edtb = . ;
  } :NONE

  .boot : AT(LOADADDR(.dtb) + SIZEOF(.dtb)) {
    _sboot = . ;
    *(.boot*) ;
    . = ALIGN(0x10) ;
    _boot_stack_end = . ;
    . += 0x100 ;
    _boot_stack_top = . ;
    . = ALIGN(0x1000) ;
    _eboot = . ;
  } :boot

  . = ALIGN(2M) ;
  _start_phys = . ;

  _k_offset = KERNEL_VA ;
  . = _k_offset + _start_phys ;

  _start = . ;
  
  .ptable (NOLOAD) : AT(_start_phys) {
    _sptable_phys = LOADADDR(.ptable) ;
    _sptable = . ;
    . += 1M ;
    _eptable_phys = LOADADDR(.ptable) + SIZEOF(.ptable) ;
    _eptable = . ;
  } :NONE

  .vtable : {
    _svtable = . ;
    KEEP(*(.vtable*)) ;
    . = ALIGN(0x1000) ;
    _evtable = . ;
  } :vtable

  .text ALIGN(0x1000) : {
    _stext = . ;
    *(.text*) ;
    _etext = . ;
  } :text

  .data ALIGN(0x4) : {
    _sdata = . ;
    *(.data*) ;
    _edata = . ;
  } :data

  .rodata ALIGN(0x4) : {
    _srodata = . ;
    *(.rodata*) ; 
    . = ALIGN(0x8) ;
    QUAD(0) ;
    _erodata = . ;
  } :rodata

  .bss ALIGN(0x4) (NOLOAD) : {
    . = ALIGN(0x10) ;
    _sbss = . ;
    *(.bss*) ;
    . = ALIGN(0x10) ;
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
