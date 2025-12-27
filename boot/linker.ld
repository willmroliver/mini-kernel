KERNEL_VMA = 0xffff000000000000 ;

PHDRS
{
  boot   PT_LOAD FLAGS(RX) ;
  text   PT_LOAD FLAGS(RX) ;
  data   PT_LOAD FLAGS(RW) ;
  rodata PT_LOAD FLAGS(R)  ;
}

ENTRY(_sboot)

SECTIONS
{
  . = LOAD_ADDRESS ;
  _ram_start = . ;

  .boot ALIGN(0x1000) : AT(LOAD_ADDRESS) {
    _sboot = . ;
    *(.boot*) ;
    . = ALIGN(0x1000) ;
    _eboot = . ;
  } :boot

  .boot_stack (NOLOAD) : {
    _boot_stack_end = . ;
    . = . + 0x4000 ;
    _boot_stack_top = . ;
  } :NONE

  .tables (NOLOAD) : {
    . = ALIGN(0x1000) ; 
    _pt_start = . ;
    . += 0x10000 ;
    _pt_end = . ;
  } :NONE

  . = KERNEL_VMA ;
  _skernel = . ;
  _skernel_pa = LOADADDR(.text) ;

  .text ALIGN(0x1000) : AT(ADDR(.boot) + SIZEOF(.boot)) {
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

  .bss (NOLOAD) : {
    _sbss = . ;
    *(.bss*) ;
    _ebss = . ;
  } :data

  _end = . ; 
}
