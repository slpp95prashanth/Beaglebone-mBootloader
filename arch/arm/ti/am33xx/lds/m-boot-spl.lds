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
    *(.text) *(.rodata*)
  } >.sram
 . = ALIGN(4);
 .data :
  {
    *(.data*)
  }
}
