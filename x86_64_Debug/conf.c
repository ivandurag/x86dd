// vi: set ai ts=4 sw=4 : -*- tab-width:4 c-basic-offset:4 -*-
// vim: set comments= cinoptions=\:0,t0,+8,c4,C1 :
/****************************************************************************
(C) Copyright 2023 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

/*
	conf.c -- Kernel configuration file.
	The data structures in here define a particular build of the
	LynxOS kernel. They take on values depending on how the kernel
	is built, which in turn depends on the kernel configuration.
*/

#if !defined(NOT_DOCUMENTED_IN_DOXYGEN)
// If BOOT_DEBUG is defined here, -DBOOT_DEBUG must be specified in BSP_RULES
#define BOOT_DEBUG
#undef BOOT_DEBUG

#define DEBUG
#undef DEBUG
#endif

/*
	The uparam.h header must be first in the header list.
	Please do not insert any headers before uparam.h.
*/
#include <uparam.h>			// KGUARDSIZE, KSTACKSIZE, NTHREADS, ...

#include <debugdef.h>		// debug()
#include <kernel.h>			// PAGESIZE, ...
#include <kas.h>			// kas_t
#include <io.h>				// struct udriver_entry
#include <kern_proto.h>		// sysbrk()
#include <globmem.h>		// nrlocks, ...
#include <hwmark.h>			// HWMID_PROCESSES, ...
#include <board.h>			// KKPF_PORT_ADDR
#include <osstacks.h>		// osstack_region_t
#include <csp_init.h>		// struct l_csp
#include <bsp_proto.h>		// struct l_bsp
#include <io/tty8250.h>		// kputc_16550()
#include <pipe.h>			// pclose(), pioctl(), ...
#include <clock_impl.h>		// __clock_getcpuclockid_mode
#include <console.h>

#include <sysdevices.h>		// CONSOLE_DEVICE
#include <conf_kas.h>		// KAS_*

// Verify KAS_*_SIZE.
_Static_assert(KAS_OSBASE_SIZE <= (2 * GB), "OSBASE size is too big");
_Static_assert(KAS_IOBASE_SIZE <= TB, "IOBASE size is too big");
_Static_assert(KAS_PHYSBASE_SIZE <= (512 * GB), "PHYSBASE size is too big");
// PHYSBASE must be at least 512G-aligned
_Static_assert((KAS_PHYSBASE & ((512 * GB) - 1)) == 0,
		"KAS_PHYSBASE is misaligned");
_Static_assert((KAS_PHYSBASE_SIZE & (PAGESIZE - 1)) == 0,
		"KAS_PHYBASE_SIZE is not 4k-aligned");
_Static_assert((KAS_OSBASE_SIZE & (PAGESIZE - 1)) == 0,
		"KAS_OSBASE_SIZE is not 4k-aligned");
_Static_assert((KAS_IOBASE_SIZE & (PAGESIZE - 1)) == 0,
		"KAS_IOBASE_SIZE is not 4k-aligned");

// Kernel Address Space regions.
volatile kas_t kas = {
	.kas_physbase		= KAS_PHYSBASE,
	.kas_physbase_size	= KAS_PHYSBASE_SIZE,
	.kas_drambase		= KAS_DRAMBASE,
	.kas_drambase_size	= KAS_DRAMBASE_SIZE,
	.kas_iobase			= KAS_IOBASE,
	.kas_iobase_size	= KAS_IOBASE_SIZE,
	.kas_perlimit		= KAS_PERLIMIT,
	.kas_perlimit_size	= KAS_PERLIMIT_SIZE,
	.kas_osbase			= KAS_OSBASE,
	.kas_osbase_size	= KAS_OSBASE_SIZE,
	.kas_specpage		= KAS_SPECPAGE,
	.kas_specpage_size	= KAS_SPECPAGE_SIZE,
	.kas_osstacks		= KAS_OSSTACKS,
	.kas_osstacks_size	= KAS_OSSTACKS_SIZE,
	.kas_uspace			= KAS_USPACE,
	.kas_uspace_size	= KAS_USPACE_SIZE
};

// OS stacks region













size_t kstacksize = KSTACKSIZE;














size_t kguardsize = KGUARDSIZE;





























































size_t
kstack_sig_lwm_get(void)
{
	return KSTACK_SIG_DELIVERY_LWM;
}














int console_device = CONSOLE_DEVICE;















uint32_t boot_profile = BOOT_PROFILE;

// Always use pre-allocated LSK PCI resources
bool pcialloc_bios_mode = true;

// If PCI_MSI is set to 0, MSI/MSI-X is disabled.
bool pci_msi_enabled = PCI_MSI;

// CONFIG.h is produced by config and defines the drivers in the system.
#include <CONFIG.h>

// Other configuration tables.
#include <smem.h>			// Lynx's own shared memory facilities.

// Devc table defines file operations for different sorts of files.
struct devc_entry devc_table[] = {
	/* Disk files */
	{
		dfread, dfwrite, dfioctl, dflseek, ionull,
		dfclose, ionull
	},
	/* Character device files */
	{
		chread, chwrite, chioctl, dflseek, chselect,
		chclose, chopen
	},
	/* Pipes and FIFOs */
	{
		pread, pwrite, pioctl, plseek, pselect,
		pclose, ppopen
	},
	/* Block device files */
	{
		ddread, ddwrite, ddioctl, dflseek, ionull,
		ddclose, ddopen
	},
#if (BUILD_MODE == DEVELOPMENT)
	/* Contiguous files */
	{
		cfread, cfwrite, dfioctl, dflseek, ionull,
		dfclose, ionull
	},
#else
	{
		ioerr_st, ioerr_st, ioerr, ionull_ll, ionull,
		ionull, ioerr
	},
#endif
	/* UNIX domain sockets */
	{
		skread, skwrite, skioctl, plseek, skselect,
		skclose, ioerr
	},
	/* Non-UNIX domain sockets */
	{
		nux_read, nux_write, nux_ioctl, ionull_ll,nux_select,
		nux_close, ioerr
	},
	/* NFS files */
#if defined(VMOS_NFS)
	{
		nfs_read, nfs_write, ioerr, nfs_seek, ionull,
		nfs_close, ionull
	},
#else
	{
		ioerr_st, ioerr_st, ioerr, ionull_ll, ionull,
		ionull, ioerr
	},
#endif
#if defined(UNSUPPORTED)
	/* POSIX.4 Draft 9 persistent IPC files */
	{
		ioftype, ioftype, ioftype, ioftype_ll, ioftype,
		ioerr, ioerr
	},
	/* POSIX.4 Draft 9 non-persistent IPC files */
	{
		ioftype, ioftype, ioftype, ioftype_ll, ioftype,
		ioerr, ioerr
	},
#else
	{
		ioerr_st, ioerr_st, ioerr, ionull_ll, ionull,
		ioerr, ioerr
	},
	/* POSIX.4 Draft 9 non-persistent IPC files */
	{
		ioerr_st, ioerr_st, ioerr, ionull_ll, ionull,
		ioerr, ioerr
	},
#endif /* UNSUPPORTED */

#if defined(UNSUPPORTED)
	/* STREAMS */
	{
		str_read, str_write, str_ioctl, ionull_ll, str_select,
		str_close, str_open
	},
#else
	{
		ioerr_st, ioerr_st, ioerr, ionull_ll, ionull,
		ionull, ioerr
	},
#endif /* UNSUPPORTED */
	/* POSIX.4 Draft 14 shared memory "files" */
	{
		ioerr_st, ioerr_st, ioerr, ionull_ll, ionull,
#if !defined(DISABLE_IPC)
		shm_close, ionull
#else
		ionull, ionull
#endif
	}
};

/************************************************************************
	Device Driver Configuration
************************************************************************/














struct bdevsw_entry
		bdevsw[(sizeof(ubdevsw) / sizeof(struct udevsw_entry)) + NBDEVS];














struct cdevsw_entry
		cdevsw[(sizeof(ucdevsw) / sizeof(struct udevsw_entry)) + NCDEVS];














struct driver_entry
		drivers[(sizeof(udrivers) / sizeof(struct udriver_entry))
		+ (sizeof(fmod_sw) / sizeof(struct fmodsw)) + NDRIVS];

#if (BUILD_MODE == DEVELOPMENT)
int cdevopen(void)
{
	return (USECDEV);
}
#endif














uint32_t num_umass_devs;

/************************************************************************
	Other Operating System Configuration
		- Configurable Modules
		- Numeric Parameters
************************************************************************/


/************************************************************************
	Numeric Parameters
************************************************************************/














#if ((USR_NFDS < 1) || (USR_NFDS > INT_MAX))
#error "Invalid USR_NFDS specified."
#endif
int NFDS = USR_NFDS;














#if ((MAXSYMLINKS < 0) || (MAXSYMLINKS > INT_MAX))
#error "Invalid MAXSYMLINKS specified."
#endif
int maxsymlinks = MAXSYMLINKS;














struct rtime_entry *proc_timers[NPROC];














uint32_t face_version = FACE_VERSION;














int system_nproc = NPROC;














int system_nstasks = NTHREADS;















#if (NBDEVS < 0)
#error "Invalid NBDEVS specified."
#endif
int nbdevs = (sizeof(ubdevsw) / sizeof(struct udevsw_entry)) + NBDEVS;














#if (NCDEVS < 0)
#error "Invalid NCDEVS specified."
#endif
int ncdevs = (sizeof(ucdevsw) / sizeof(struct udevsw_entry)) + NCDEVS;














#if (NDRIVS < 0)
#error "Invalid NDRIVS specified."
#endif
int ndrivs = (sizeof(udrivers) / sizeof(struct udriver_entry))
		+ (sizeof(fmod_sw) / sizeof(struct fmodsw)) + NDRIVS;


















#if ((VM0_PERMAP_SIZE < 0) || (VM0_PERMAP_SIZE > KAS_PERLIMIT_SIZE))
#error "Invalid VM0_PERMAP_SIZE specified."
#endif
size_t vm0_permap_size = VM0_PERMAP_SIZE;

/************************************************************************
	Numeric parameters for POSIX Configuration
************************************************************************/

/*
	Number of clocks allowed on the system.
	The system clock itself counts as one.
*/
int Sys_nclock = NCLOCK;	/* total possible number hardware clocks */
struct clock_descr clock_table[NCLOCK];

int sigqueue_max = SIGQUEUE_MAX;

/*
	The NONPOSIX_SA_HANDLER_PROTO constant makes it
	possible to provide signal-catching function with 'info' and
	'context' input parameters even if SA_SIGINFO flag
	is not set.
*/
#if defined(NONPOSIX_SA_HANDLER_PROTO)
int non_posix_siginfo = NONPOSIX_SA_HANDLER_PROTO;
#else
int non_posix_siginfo = (int)0;
#endif

/*
	The NONPOSIX_CLOCK_NANOSLEEP constant causes
	clock_nanosleep() to return -1 when interrupted by signal.
	This was the default behavior prior to the 2023.10 release.
*/
#if defined(NONPOSIX_CLOCK_NANOSLEEP)
bool non_posix_clock_nanosleep = NONPOSIX_CLOCK_NANOSLEEP;
#else
bool non_posix_clock_nanosleep = false;
#endif

/************************************************************************
	Numeric parameters for Generic POSIX.4 Configuration
************************************************************************/

#if (CBLKSIZE < 0) || (CBLKSIZE > 6)
#warning "Invalid CBLKSIZE specified. Defaulting to 2K."
int cblksize = (int)2;
#else
int cblksize = CBLKSIZE;
#endif














#if ((NUMIOINTS < 1) || (NUMIOINTS > MAXIOINTS))
#error "Invalid NUMIOINTS specified."
#endif
uint32_t numioints = NUMIOINTS;














#if ((NUMIOVECS <= TIMERVEC) || (NUMIOVECS > EXCP_NUM))
#error "Invalid NUMIOVECS specified."
#endif
uint32_t numiovecs = NUMIOVECS;

int numktimers = NTIMERS;

int default_quantum = QUANTUM;














int tickspersec = TICKSPERSEC;














nseconds_t nsecspertick = 1000000000U / TICKSPERSEC;

/* table of real time timers */
struct rtime_entry rtimer_table[NTHREADS];














struct pentry *proctab;

int reboot_on_panic = REBOOT_ON_PANIC;
int ngroups = NGROUPS;

/************************************************************************
	Machine-specific configuration
************************************************************************/














unsigned long *page_tables[NPROC];

#if (BUILD_MODE == DEVELOPMENT)
int hm_console = HM_CONSOLE;
int hm_catch_faults = HM_CATCH_FAULTS;
// klog stuff.
static int
__dummy_put_klog(char *msg)
{
	return OK;
}
int (*put_klog)() = &__dummy_put_klog;
#endif // BUILD_MODE == DEVELOPMENT

/************************************************************************
	SKDB configuration stuff
************************************************************************/
char skdb_key_serial = SKDB_KEY_SERIAL;	/* These are usualy the same */
char skdb_brk_char = SKDB_BRK_CHAR;		/* But this is used by remote.c */

/*---------------------------------------------------------------------------*/
extern int skdb_install();				/* This is from kdb.c */
void attach_skdb(void)
{
#if defined(INSTALL_SKDB)
	skdb_install();
#endif /* ! INSTALL_SKDB */
}

#define COM1				UART0_BASE
#define COM2				UART1_BASE
kaddr_t kkpf_port_addr = COM1;

void (*kputc)(int32_t, kaddr_t) = kputc_16550;




































































































































































void
bsp_set_VM0_limits(void)
{
	// Log the max sysram available.
	maximum_resource((int)0, HWMID_SYSRAM, Sram_Size);

	// Initilize FS Vm0 parameters from uparam.h defines.
	cacheblks = CACHEBLKS;
	nfiles = NFILES;
	ninodes = NINODES;
	nmounts = LYNX_MOUNTS + NMOUNTS;
	nrlocks = NRLOCKS;
	write_through = WRITE_THROUGH;

	// Initilize process and thread parameters for VM0 from uparam.h defines.
	nproc	= VMZERO_NPROC;
	nstasks	= VMZERO_NTHREADS;
	maximum_resource((int)0, HWMID_PROCESSES, VMZERO_NPROC);
	maximum_resource((int)0, HWMID_THREADS, VMZERO_NTHREADS);

	// Start of NAME_SERVER.C global data.
	num_names = NUM_NAMES;
	num_name_opens = NUM_NAME_OPENS;
	name_buf_size = num_names * AVG_NAME_LEN;
	nmsgq = VMZERO_MSGQS;
	nsems = VMZERO_SEMAPHORES;
	debug(("Init NAME_SERVER.C names=%d name_opens=%d\n", num_names,
			num_name_opens));

	// Start of pshmem.c global data.
	nshm = NSHM;
	debug(("Init PSHMEM.C shm=%d\n", nshm));

	// Start of pipe.c global data.
	npipes = NPIPES;
	debug(("Init PIPE.C pipes=%d\n", npipes));

	nsockets = NSOCKETS;
	debug(("Init nsockets=%d\n", nsockets));

	// Start of threads.c global data.
	nugStackSem = (int)1;
	nugStackTop = (unsigned long *)sysbrk(CTX_EXITSTACK_SIZE
			* sizeof(unsigned long));
	if (nugStackTop == NULL) {
		panic("Failed to allocate nugStackTop");
	}

	debug(("Init THREADS.C nugStackBeg=0x%x\n", nugStackTop));
	nugStackTop = nugStackTop + CTX_EXITSTACK_SIZE;
	debug(("Init THREADS.C nugStackTop=0x%x\n", nugStackTop));

	// Start of sigvec.c global data.
	nsignotes = NSIGNOTES;
	debug(("Init SIGVEC.C signotes=%d\n", nsignotes));

	// Start of fast_ada.c global data.
	Max_fast_sems = MAX_FAST_SEMS;
	pi_init(&Fast_sem_get_sem);		// used only by fast_sem_get()
	debug(("Init FAST_ADA.C #Max_fast_sems=%d\n", Max_fast_sems));

	numtouts[0] = NUMTOUTS;

	__clock_getcpuclockid_mode = CLOCK_GETCPUCLOCKID_MODE;

	debug(("VM0 Limits Set Complete\n"));

	// Clear the super fast ready queue for VM0.
	vm_currtptr[0] = NULL;
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

/*
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
*/

// End of file.
