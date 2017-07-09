OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x0000;
SECTIONS
{
  .text phys : AT(phys) {
    code = .;
    *(.text.start);
    *(.text*)
    *(.rodata)
    . = ALIGN(4);
  }
  __text_end=.;
  .data : AT(phys + (data - code))
  {
    data = .;
    *(.data)
    . = ALIGN(4);
  }
  __data_end=.;
  .bss : AT(phys + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4);
  }
  __binary_end = .;
}
