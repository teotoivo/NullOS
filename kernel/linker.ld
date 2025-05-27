ENTRY(start)

PHDRS
{
	text PT_LOAD FLAGS(5); /* R + X = 4 + 1 */
	data PT_LOAD FLAGS(6); /* R + W = 4 + 2 */
}

SECTIONS
{
	. = 1M;

	.boot : ALIGN(4K)
	{
		KEEP(*(.multiboot_header))
	} :text

	.text : ALIGN(4K)
	{
		*(.text*)
		*(.init*)
	} :text

	.rodata : ALIGN(4K)
	{
		*(.rodata*)
	} :text

	.data : ALIGN(4K)
	{
		*(.data*)
	} :data

	.bss : ALIGN(4K)
	{
		*(.bss*)
		*(COMMON)
	} :data
}

