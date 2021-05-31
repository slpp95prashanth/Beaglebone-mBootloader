MEMORY { .sram : ORIGIN = 0x402F0400, LENGTH = (64 * 1024) }
MEMORY { .sdram : ORIGIN = 0x80000000, LENGTH = 0x80000 }
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
 .text :
  {
  _start = .;
  arch/arm/cpu/armv7/start.o (.text)
  arch/arm/cpu/armv7/lib.o (.text)
    *(.text) *(.rodata*) *(.vectors)
  } >.sram

 . = ALIGN(4);

 .bss :
  {
    _BSS_START_ = .;
    *(.bss*)
    . = ALIGN(4);
    _BSS_END_ = .;
  } >.sram

 . = ALIGN(4);

 .data :
  {
    *(.data*)
  } >.sram
}
