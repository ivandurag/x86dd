/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
/****************************************************************************
(C) Copyright 2023 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

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
**   conf_am.c
**
** PURPOSE:
**   Implements configuration data initialization functions executed 
**   BEFORE the MMU is turned on (ante-MMU).
**
** NOTES:
**   Only automatic variables and access to the global data
**   pointed to by 'lvar_ptr' are allowed.
**
** ABBREVIATIONS/ACRONYMS:
**   None.
**
************************************************************************
.FP END
*/

#include <csp_init.h>		// struct l_csp
#include <bsp_proto.h>		// struct l_bsp
#include <conf_kas.h>		// KAS_*

#if !defined(NOT_DOCUMENTED_IN_SRD)





































kaddr_t
kas_get_osbase_ea(void)
{
	return KAS_OSBASE;
}






































kaddr_t
kas_get_osbase_size_ea(void)
{
	return KAS_OSBASE_SIZE;
}


















































































void
kas_pre_init_common(l_vars_t *lvar_ptr)
{
	kas_t *lkas = &lvar_ptr->l_common.kas;

	// Correct memory size
	if (lvar_ptr->l_common.mem_size > KAS_PHYSBASE_SIZE) {
		lvar_ptr->l_common.mem_size = KAS_PHYSBASE_SIZE;
	}

	// Set KAS values.
	lkas->kas_physbase      = KAS_PHYSBASE;
	lkas->kas_physbase_size = KAS_PHYSBASE_SIZE;
	lkas->kas_iobase        = KAS_IOBASE;
	lkas->kas_iobase_size   = KAS_IOBASE_SIZE;
	lkas->kas_perlimit      = KAS_PERLIMIT;
	lkas->kas_perlimit_size = KAS_PERLIMIT_SIZE;
	lkas->kas_osbase        = KAS_OSBASE;
	lkas->kas_osbase_size   = KAS_OSBASE_SIZE;
	lkas->kas_specpage      = KAS_SPECPAGE;
	lkas->kas_specpage_size = KAS_SPECPAGE_SIZE;
	lkas->kas_osstacks      = KAS_OSSTACKS;
	lkas->kas_osstacks_size = KAS_OSSTACKS_SIZE;
	lkas->kas_uspace        = KAS_USPACE;
	lkas->kas_uspace_size   = KAS_USPACE_SIZE;
}
#endif // !NOT_DOCUMENTED_IN_SRD

























































size_t
kstack_size_get(void)
{
	return (size_t)KSTACKSIZE;
}










































































kaddr_t
kstack_top_get(tid_t tid)
{
	return (kaddr_t)
			&(((osstack_region_t *)KAS_OSSTACKS)->kstacks[tid].kstack_top);
}

// End of file.
