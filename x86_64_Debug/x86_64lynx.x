/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
/****************************************************************************
(C) Copyright 2023 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

OUTPUT_FORMAT("elf64-x86-64-lynx", "elf64-x86-64-lynx", "elf64-x86-64-lynx")
OUTPUT_ARCH(i386:x86-64)
ENTRY(_main)

PHDRS
{
        text PT_LOAD;
        data PT_LOAD;
}


SECTIONS
{
	. = SIZEOF_HEADERS;
	.text :
	{
		/* Place for a.out header */
		/* .kboot0 is entry point, must be at 0x1000 offset*/
		*(.kboot0)
		. = ALIGN(0x1000);
		/* .kboot1 is bsp_relocate() at the same page as kboot0 */
		*(.kboot1)
		/* .kboot2 is bsp_lowInit(), and it must start at the next page */
		. = ALIGN(0x1000);
		*(.kboot2)
		*(.text)
		*(.text.*)
		/* .gnu.warning sections are handled specially by elf32.em. */
		*(.gnu.warning)
		*(.gnu.linkonce.t*)
		. = ALIGN(16);
	} :text =0x9090
	. = ALIGN(0x8);
	__ctors_start = .;
	.ctors :
	{
		*(.ctors)
		*(.ctors.*)
	}
	__ctors_end = .;
	. = ALIGN(16);
	.rodata	:
	{
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r*)
	} :text
	.rodata1 : { *(.rodata1) }
	/* map_kernel() expects data to start at the next page. */
	. = ALIGN(0x1000);
	_etext = .;
	PROVIDE (etext = .);
	.data :
	{
		*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
		. = ALIGN(16);
	} :data
	.data1 : { *(.data1) }
	/*
		We want the small data sections together, so single-instruction offsets
		can access them all, and initialized data all before uninitialized, so
		we can shorten the on-disk segment size.
	*/
	.sdata : { *(.sdata) }
	_edata = .;
	PROVIDE (edata = .);
	__bss_start = .;
	.bss :
	{
		/* Global memory page must combine all globmem.c symbols in one page */
		. = ALIGN(0x1000);
		gstaddr = .;
		*(.bss.globmem)
		. = ALIGN(0x1000);
		gendaddr = .;
		*(.dynbss)
		*(.bss)
		*(COMMON)
	}
	.sbss :
	{
		PROVIDE (__sbss_start = .);
		*(.sbss)
		*(.scommon)
		*(.dynsbss)
		PROVIDE (__sbss_end = .);
	}
	. = ALIGN(32 / 8);
	_end = .;
	PROVIDE (end = .);
	/* Stabs debugging sections. */
	.stab				0 : { *(.stab) }
	.stabstr			0 : { *(.stabstr) }
	.stab.excl			0 : { *(.stab.excl) }
	.stab.exclstr		0 : { *(.stab.exclstr) }
	.stab.index			0 : { *(.stab.index) }
	.stab.indexstr		0 : { *(.stab.indexstr) }
	.comment			0 : { *(.comment) }
	/*
		DWARF debug sections.
		Symbols in the DWARF debugging sections are relative to the beginning
		of the section so we begin them at 0.
	*/
	/* DWARF 1 */
	.debug				0 : { *(.debug) }
	.line				0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo		0 : { *(.debug_srcinfo) }
	.debug_sfnames		0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges		0 : { *(.debug_aranges) }
	.debug_pubnames		0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info			0 : { *(.debug_info) }
	.debug_abbrev		0 : { *(.debug_abbrev) }
	.debug_line			0 : { *(.debug_line) }
	.debug_frame		0 : { *(.debug_frame) }
	.debug_str			0 : { *(.debug_str) }
	.debug_loc			0 : { *(.debug_loc) }
	.debug_macinfo		0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames	0 : { *(.debug_weaknames) }
	.debug_funcnames	0 : { *(.debug_funcnames) }
	.debug_typenames	0 : { *(.debug_typenames) }
	.debug_varnames		0 : { *(.debug_varnames) }
	/* These must appear regardless of . */
}

/* End of file. */
