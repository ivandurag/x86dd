// vi: set ai ts=4 sw=4 : -*- tab-width:4 c-basic-offset:4 -*-
// vim: set comments= cinoptions=\:0,t0,+8,c4,C1 :
/****************************************************************************
(C) Copyright 2023 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

#include <kernel.h>			// SYSCALL, SYSERR, ...
#include <errno.h>			// EINVAL
#include <conf_kas.h>		// KAS_*
#include <io.h>				// drivers[]
#include <globmem.h>		// Name_table, ...
#include <sys/socket.h>		// Tcpip_info
#include <lynx/info.h>		// I_PROCTAB etc.
#include <bsp.h>			// sc_info_t
#include <kparam.h>			// NCLOCK
#include <conf.h>			// nfiles, face_version, num_umass_devs

// Initialize only those entries that can be initialized using constants.
static long info_table[I_INFO_NUM] = {
	[I_DRIVERS] = (long)drivers,
	[I_CDEVSW] = (long)cdevsw,
	[I_BDEVSW] = (long)bdevsw,
	[I_NPROCTAB] = (long)NPROC,
	[I_FREEPAGES] = (long)&freepages,
	[I_NTHREADS] = (long)NTHREADS,
	[I_INODETABLE] = (long)&inode_table,
	[I_EXECARGLEN] = (long)EXECARGLEN,
	[I_HZ] = (long)LHZ,
	[I_NGROUPS] = (long)NGROUPS,
	[I_USR_NFDS] = (long)USR_NFDS,
	[I_LMAX_LINKS] = (long)LMAX_LINKS,
	[I_LINELEN] = (long)LINELEN,
	[I_QSIZE] = (long)QSIZE,
	[I_MAXNAMLEN] = (long)MAXNAMLEN,
	[I_MAXPATHLEN] = (long)MAXPATHLEN,
	[I_PIPESIZE] = (long)PIPESIZE,
	[I_FILE_TABLE] = (long)&file_table,
	[I_FILE_FREE] = (long)&file_free,
	[I_MOUNTTABLE] = (long)&mount_table,
	[I_PHYSBASE] = (long)KAS_PHYSBASE,
#if defined(_POSIX_MEMLOCK)
	[I_SC_MEMLOCK] = 1L,
#endif
#if defined(_POSIX_PRIORITY_SCHEDULING)
	[I_SC_PRIORITY_SCHEDULING] = 1L,
#endif
#if defined(_POSIX_SHARED_MEMORY_OBJECTS)
	[I_SC_SHARED_MEMORY_OBJECTS] = 1L,
#endif
	[I_SYS_NCLOCK] = (long)NCLOCK,
	[I_MAXSYMLINKS] = (long)MAXSYMLINKS,
	[I_CURRPSS] = (long)KAS_SPECPAGE,
	[I_OSBASE] = (long)KAS_OSBASE,
	[I_KAS_ADDR] = (long)&kas,
	[I_BP_INFO] = (long)&bp_info,
	[I_FACE_VERSION] = (long)FACE_VERSION,
	[I_NDDRIVERS] = (long)NDRIVS,
	[I_NDCDEVSW] = (long)NCDEVS,
	[I_NDBDEVSW] = (long)NBDEVS
};
















































































void
info_init(void)
{
	// proctab is dynamically allocated.
	info_table[I_PROCTAB] = (long)proctab;

	// Entries below are calculated at system start.
	info_table[I_NDRIVERS] = (long)ndrivs;
	info_table[I_NCDEVSW] = (long)ncdevs;
	info_table[I_NBDEVSW] = (long)nbdevs;

	// Entries below are dynamically allocated.
	info_table[I_THREADTAB] = (long)st_table;
	info_table[I_TCPIP_INFO] = (long)Tcpip_info;
}
































































































































#undef __ksc_info
int __ksc_info(info_t) ATTR_WEAK_ALIAS(__info);

// __info FKA info
int
__info(info_t num)
{
	if ((num < 0) || (num >= I_INFO_NUM)) {
		pseterr(EINVAL);
		return SYSERR;
	}

	switch (num) {
		// VM-specific values.
		case I_NUM_INODES:
			return ninodes;
		case I_NFILES:
			return nfiles;
		case I_NMOUNTS:
			return nmounts;
		case I_SC_SEMAPHORES:
			return nsems;
		case I_NAME_TABLE:
			return (SYSCALL)Name_table;
		case I_NUM_NAMES:
			return num_names;
		// Dynamic values.
		case I_UPTIME:
			return (boot_profile != 0U) ? (SYSCALL)(time_at_last_clkint
					+ bsp_read_osticker()) : (SYSCALL)0;
		case I_NUSB_MASS:
			return (SYSCALL)num_umass_devs;
		// Constant values.
		default:
			return (SYSCALL)info_table[num];
	}
}

/*
	INFORMATION SUBJECT TO EXPORT CONTROL LAWS

	Subject to local country rules and laws when applicable, you must
	comply with the following:

	'These commodities, technology, or software were exported from the
	United States in accordance with the Export Administration
	Regulations. Diversion contrary to U. S. law and other relevant
	export controls is prohibited. They may not be re-exported to any
	of the following destinations without authorization; Cuba, Iran,
	Iraq, Libya, North Korea, Sudan or any other country to which
	shipment is prohibited; nor to end-use(r)s involved in chemical,
	biological, nuclear, or missile weapons activity.'

	COPYRIGHT NOTICE:
		(C) Copyright 2001-2002 Rockwell Collins, Inc. All rights reserved.
*/

// End of file.
