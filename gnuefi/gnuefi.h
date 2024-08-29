/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
/****************************************************************************
(C) Copyright 2022-2023 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

#if !defined(__gnuefi_h)
#define __gnuefi_h

#include <stdint.h>

#define OK							0
#define NOT_OK						1
#define MAX_ACPI_GUIDS				2
#define EFI_MAGIC_NO				0xdead
#define EFI_LOAD_ADDRESS			0x101000
#define EFI64KDI	0x64646464UL

#define STR2(x) #x
#define STR(x) STR2(x)

#define INCBIN(name, file)						\
	__asm__(".section .rodata\n"				\
		".global incbin_" STR(name) "_start\n"	\
		".balign 4096\n"						\
		"incbin_" STR(name) "_start:\n"			\
		".incbin \"" file "\"\n"				\
		".global incbin_" STR(name) "_end\n"	\
		".balign 1\n"							\
		"incbin_" STR(name) "_end:\n"			\
		".byte 0\n"								\
	);											\
extern const __attribute__((aligned(16))) void* incbin_ ## name ## _start; \
extern const void* incbin_ ## name ## _end; \

typedef struct __attribute__ ((__packed__)) {
	INT16 limit;
	UINT64 base;
} gdt_ptr_t;

typedef struct __attribute__ ((__packed__)) {
	uint16_t magic;
	uint32_t acpi_rsdp;
} gnuefi_metadata_t;

struct __attribute__ ((__packed__)) {
	UINT64  entry;
	UINT16  cs;
} ljmp_info;

#endif // __gnuefi_h

