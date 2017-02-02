OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	.text : 
	{
		*start940.o(.text .stub .text.* .gnu.linkonce.t.*)
		*(.text .stub .text.* .gnu.linkonce.t.*)
	} = 0

	.ctors :
	{
		__CTOR_LIST__ = .;
		KEEP (*(SORT(.ctors.*)))
		KEEP (*(.ctors))
		LONG(0)
	}
	
	.dtors :
	{
		__DTOR_LIST__ = .;
		KEEP (*(SORT(.dtors.*)))
		KEEP (*(.dtors))
		LONG(0)
	}
	
	.rodata  : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
	.rodata1 : { *(.rodata1) }

	.data :
	{
		*(.data .data.* .gnu.linkonce.d.*)
		KEEP (*(.gnu.linkonce.d.*personality*))
		SORT(CONSTRUCTORS)
	}
	.data1          : { *(.data1) }

	. = ALIGN(4);
	__bss_start__ = .;
	.bss :
	{
		*(.dynbss)
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(4);
	}
	. = ALIGN(4);
	__bss_end__ = . ;
	
	/DISCARD/ : { *(*) }
}
