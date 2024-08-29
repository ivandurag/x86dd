/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
/****************************************************************************
(C) Copyright 2022-2023 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

#include <efi.h>
#include <efilib.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include "gnuefi.h"

// GDT for switching to compatibility mode
UINT32 gdt[] __attribute__((aligned (8))) = {
	// Null descriptor (for unused selectors: GS, FS)
	0, 0,

	// Selector 0x0008 (CODE32SEL), 4GiB 32-bit code segment
	0x0000ffff, 0x00cf9a00,

	// Selector 0x0010 (DATA32SEL): 4GiB 32-bit data segment
	0x0000ffff, 0x00cf9200,
};

INCBIN(lynxos178, "lynxos-178.kdi");

typedef struct {
	UINTN map_key;
	UINTN map_entries;
	UINTN desc_size;
	UINTN map_actual_size;
	UINTN map_alloc_size;
	EFI_MEMORY_DESCRIPTOR *mem_map;
} uefi_mm_info_t;
/* UEFI memory map information */
static uefi_mm_info_t uefi_mm_info;

typedef struct {
	uint64_t addr;
	uint64_t size;
} physmap_entry_t;

/// Maximum number of entries in system memory map
#define MEMORY_MAP_ENTRIES     510

struct {
	size_t count;
	physmap_entry_t mem_map[MEMORY_MAP_ENTRIES];
} physmap_table;

/**
	Retrieve current memory map from UEFI firmware.
	Populate uefi_mm_info global structure with the current UEFI memory
	map information.

	@return status_t (OK: Success, NOT_OK: Error)
*/
static EFI_STATUS
get_uefi_mem_map(void)
{
	UINT32 desc_ver;
	EFI_STATUS efi_status;
	EFI_MEMORY_DESCRIPTOR *uefi_map_ptr = NULL;
	int i;

	memset(&physmap_table, 0, sizeof(physmap_table));

	if (uefi_mm_info.map_key == 0) { /* First time invocation */
		/* Retrieve the map size, before retrieving the map */
		uefi_mm_info.map_actual_size = 0;

		efi_status = uefi_call_wrapper(BS->GetMemoryMap, 5,
			&uefi_mm_info.map_actual_size, uefi_mm_info.mem_map,
			&uefi_mm_info.map_key, &uefi_mm_info.desc_size, &desc_ver);
		if (efi_status != EFI_BUFFER_TOO_SMALL) {
			Print(L"GetMemoryMap() failed while retrieving map size!\n");
			return efi_status;
		}
		uefi_mm_info.map_alloc_size = uefi_mm_info.map_actual_size
										+ EFI_PAGE_SIZE;

		uefi_mm_info.mem_map = AllocatePool(uefi_mm_info.map_alloc_size);
		if (uefi_mm_info.mem_map == NULL) {
			Print(L"AllocatePool() failed!\n");
			return EFI_OUT_OF_RESOURCES;
		}
	}

	uefi_mm_info.map_actual_size = uefi_mm_info.map_alloc_size;

	/* Retrieve the current memory map from UEFI firmware */
	efi_status = uefi_call_wrapper(BS->GetMemoryMap, 5,
			&uefi_mm_info.map_actual_size, uefi_mm_info.mem_map,
			&uefi_mm_info.map_key, &uefi_mm_info.desc_size, &desc_ver);
	if (EFI_ERROR(efi_status)) {
		Print(L"GetMemoryMap() failed with error %r!\n", efi_status);
		return efi_status;
	}

	uefi_mm_info.map_entries = uefi_mm_info.map_actual_size
			/ uefi_mm_info.desc_size;

	for (i = 0, uefi_map_ptr = uefi_mm_info.mem_map;
			i < uefi_mm_info.map_entries; i++) {

		if (uefi_map_ptr->Type == EfiConventionalMemory) {
			physmap_table.mem_map[physmap_table.count].addr
					= uefi_map_ptr->PhysicalStart;
			physmap_table.mem_map[physmap_table.count].size
					= uefi_map_ptr->NumberOfPages * EFI_PAGE_SIZE;
			physmap_table.count ++;
		}
		uefi_map_ptr = NextMemoryDescriptor(uefi_map_ptr,
				uefi_mm_info.desc_size);
	}
	return EFI_SUCCESS;
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
	EFI_SYSTEM_TABLE *SystemTable)
{
	gdt_ptr_t gdt_ptr;
	void (*func)(void *);

	InitializeLib(ImageHandle, SystemTable);

	gdt_ptr.base = (UINT64)gdt;
	gdt_ptr.limit = sizeof(gdt);

	memcpy((void *)EFI_LOAD_ADDRESS, &incbin_lynxos178_start,
		(size_t)&incbin_lynxos178_end - (size_t)&incbin_lynxos178_start + 1);

	// if EFI64 KDI - do not jump to 32bit mode
	if (*(UINT32 *)EFI_LOAD_ADDRESS == EFI64KDI) {
		func = (void (*)(void *))(EFI_LOAD_ADDRESS + 0x20);
		if (get_uefi_mem_map() != EFI_SUCCESS) {
			Print(L"efistub panic: cannot retrieve memory map\n");
			for (;;);
		}
		if (physmap_table.count == 0) {
			Print(L"efistub panic: memory map contains zero entries\n");
			for (;;);
		}
		func(&physmap_table);
	} else {

		ljmp_info.entry = EFI_LOAD_ADDRESS  + 0x20;
		ljmp_info.cs = 0x8;

		asm volatile (
			"cli;"
			"lgdt %0;"
			"rex.w ljmp *(%1);"
			"hlt;"
			:: "m"(gdt_ptr), "r"(&ljmp_info));
	}
	return EFI_SUCCESS;
}

