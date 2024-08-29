/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
#if !defined(__conf_kas_h)
#define __conf_kas_h
/****************************************************************************
(C) Copyright 2023 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

//-------------------------- FILE PROLOGUE ---------------------------

/*
.FP
************************************************************************
**
** INFORMATION SUBJECT TO EXPORT CONTROL LAWS
**
** Subject to local country rules and laws when applicable, you must
** comply with the following:
**
** 'These commodities, technology, or software were exported from the
** United States in accordance with the Export Administration
** Regulations.  Diversion contrary to U. S. law and other relevant
** export controls is prohibited.   They may not be re-exported to any
** of the following destinations without authorization; Cuba, Iran,
** Iraq, Libya, North Korea, Sudan or any other country to which
** shipment is prohibited; nor to end-use(r)s involved in chemical,
** biological, nuclear, or missile weapons activity.'
**
** COPYRIGHT NOTICE:
**   (C) Copyright 2001-2002 Rockwell Collins, Inc.  All rights reserved.
**
** FILE NAME:
**   conf.c
**
** PURPOSE:
**   define LynxOS tunables and configuration data
**
** NOTES:
**   only changes to original LynxOS version are the definition of
**   SKDB port for the __cpr__ case
**
************************************************************************
.FP END
*/

//------------------------- FILE INCLUSION ---------------------------

#include "uparam.h"		// KAS_*_SIZE
#include <kernel.h>		// MB
#include <arch_mem.h>	// PHYS_OS_START

//------------------------ MACRO DEFINITIONS -------------------------

// PCI memory alignment: 8MB.
#define MB_OFFSET_MASK			((1 << 20) - 1)

#define KAS_OSBASE		0xffffffff80000000ULL
#define	KAS_IOBASE		0xffffa00000000000ULL
#define KAS_PERLIMIT	(KAS_IOBASE)
#define KAS_PHYSBASE	0xffff800000000000ULL

// Set Specpage size.
#define KAS_SPECPAGE_SIZE	(SPECPAGESIZE)

// Calculate Specpage start address.
#define KAS_SPECPAGE		0x00007ffffffff000ULL
// Calculate OS Stacks region size.
#define KAS_OSSTACKS_SIZE	(sizeof(osstack_region_t))

// Calculate OS Stacks region start address.
#define KAS_OSSTACKS		((KAS_SPECPAGE) - (KAS_OSSTACKS_SIZE))

// Calculate Users Space start address.
#define KAS_USPACE			(KAS_OSSTACKS)

// Calculate Users Space size.
#define KAS_USPACE_SIZE		((KAS_USPACE) - (PHYS_OS_START))

#endif // !defined(__conf_kas_h)

// End of file.
