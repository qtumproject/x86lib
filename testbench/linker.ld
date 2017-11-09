OUTPUT_FORMAT("binary")
ENTRY(start)
phys = 0x1000;
scratch = 0x100000;
MEMORY
{
  coderom (rx) : ORIGIN = phys, LENGTH = 128K
  scratchram (!rx) : ORIGIN = scratch, LENGTH = 1024K
}
SECTIONS
{
  .text phys : AT(phys) {
    code = .;
    *(.text.start);
    *(.text*)
    *(.rodata)
    . = ALIGN(4);
  } > coderom
  __text_end=.;
  .data : AT(phys + (data - code))
  {
    data = .;
    *(.data)
    . = ALIGN(4);
  } > coderom
  __data_end=.;
  __binary_end = .;
  .bss : AT(scratch)
  {
    bss = .;
    *(.bss)
    . = ALIGN(4);
  } > scratchram
}

