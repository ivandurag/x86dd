/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
#if !defined(__uparam_h)
#define __uparam_h
/****************************************************************************
(C) Copyright 2023-2024 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

//-------------------------- FILE PROLOGUE ---------------------------

/*
.FP
***********************************************************************
*
* EXPORT NOTICE:
*
*   INFORMATION SUBJECT TO EXPORT CONTROL LAWS
*
* Subject to local country rules and laws when applicable, you  
* must comply with the following:
* 
* These commodities, technology, or software were exported from  
* the United States in accordance with the Export Administration 
* Regulations.  Diversion contrary to U. S. law and other relevant 
* export controls is prohibited.   They may not be re-exported to  
* any of the following destinations without authorization; Cuba,  
* Iran, Iraq, Libya, North Korea, Sudan or any other country to  
* which shipment is prohibited; nor to end-use(r)s involved in  
* chemical, biological, nuclear, or missile weapons activity.
*
* COPYRIGHT NOTICE:
*   (C) Copyright 2001-2002 Rockwell Collins, Inc.  All rights reserved.
*
* FILE NAME:
*   uparam.h
*
* PURPOSE:
* This file contains the modifiable kernel parameters with default values.
*
* NOTES: (optional)
*
* To modify a parameter, please, use special file - "xparam.h"
* see example and instructions there.
*																
* ABBREVIATIONS/ACRONYMS:
*
*****************************************************************
.FP END
*/

//------------------------- FILE INCLUSION ---------------------------

#include <sys/param.h>		// Get default values
#include <csp_init.h>		// MAX_PROC_NUM

//------------------------ MACRO DEFINITIONS -------------------------

#define FACE_VERSION		0

#define KAS_PHYSBASE_SIZE	(512ULL * GB)
#define KAS_OSBASE_SIZE		(2ULL * GB)
#define KAS_PERLIMIT_SIZE	(8ULL * GB)
#define KAS_IOBASE_SIZE		(1ULL * GB)
// Address of RAM in physical 4G address space.
#define KAS_DRAMBASE	0x0UL
// Size of RAM
#define KAS_DRAMBASE_SIZE	KAS_PHYSBASE_SIZE

// The clock_getcpuclockid() mode: CLOCK_UNSUPPORTED or CLOCK_INACCURATE.
#define CLOCK_GETCPUCLOCKID_MODE	CLOCK_INACCURATE

#define BOOT_PROFILE		0

/*
	System-wide adjustables

	NPROC		Specifies the number of struct pentry allocated to proctab
				(used by all VMs).

	NTHREADS	Specifies the number of struct st_entry allocated to st_table
				(used by all VMs).

	USR_NFDS	Limits the number of files that a single process can open.
*/
#undef NPROC
#if (BUILD_MODE == UNIKERNEL)
#define NPROC			1		// Max number of processes
#else
#define NPROC			254
#endif

#if ((NPROC < 1) || (NPROC > MAX_PROC_NUM))
#error NPROC must be less than or equal to MAX_PROC_NUM.
#endif

#undef NTHREADS
#define NTHREADS		446		// Max number of threads

#if (NTHREADS < NPROC)
#error NTHREADS must be greater than or equal to NPROC.
#endif

#undef USR_NFDS
#define USR_NFDS		2176	// Max number of open files per process

/*
   When the process table (proctab) and thread table (st_table) are created,
   no file system has been mounted, making it impossible to set VM 0 limits
   in the VCT.  To fix this we define process and thread limits for VM 0
   here.
*/
// max number of processes allowed in VM 0
#define VMZERO_NPROC			30
// max number of threads allowed in VM 0
#define VMZERO_NTHREADS		(2*VMZERO_NPROC + 4*16)
#define VMZERO_MSGQS			VMZERO_NPROC
#define VMZERO_SEMAPHORES		(VMZERO_NPROC * 5)

/* 
	On the average, each kernel thread requires around 5 pages or 20k virtual
	space, so the following default number would support around 50 kt's for
	VM0. The permap size will be computed based on the number of VMs really
	hosted by the BSP; however, this is the minimum number required for VM0.
*/
#define VM0_PERMAP_SIZE 0x00100000

/*
	Room for extra dynamically installed drivers and devices
	NBDEVS	specifies the number of struct bdevsw_entry added to bdevsw
			over and above the statically install devices
	NCDEVS	specifies the number of struct cdevsw_entry added to cdevsw
			over and above the statically install devices
	NDRIVS	specifies the number of struct driver_entry added to drivers
			over and above the statically install drivers and protocol modules
*/
// max number of dynamic block devices
#undef  NBDEVS
#define NBDEVS			16
// max number of dynamic character devices
#undef  NCDEVS
#define NCDEVS			(NDRIVS + (NDRIVS / 2))
// max # dynamic drvrs (plus STREAMS modules)
#undef  NDRIVS
#define NDRIVS			32		

/*
	File system adjustables.

	Except for the global parameter 'CBLKSIZE', the following values are for
	VM0's file system only, all other VM's have these values obtained from
	the VCT.

	NFILES		Specifies the number of struct file allocated to file_table

	NINODES		Specifies the number of struct inode_entry allocated to
				inode_table, the number of struct ihead_entry added to
				ihead_table and struct locklist ** allocated to lockedinodes

	NMOUNTS		Specifies the number of struct ihead_entry added to ihead_table

	CACHEBLKS	Specifies the number of struct buf_entry allocated to
				bhead_table and number of blocks allocated to cache_blocks.

	CBLKSIZE	COMMOM TO ALL VMs specifies the size of the blocks allocated for
				the memory pool used by bhead_table. The number of blocks is
				specified by CACHEBLKS. The size is 512 * 2^CBLKSIZE bytes.
				Valid range is 0..6. Anything outside this range will default
				to 2.
*/
#undef  NFILES
#define NFILES			2176	// Max number of open files in system
#undef  NINODES
#define NINODES			NFILES	// Max number of incore inodes
#undef  CACHEBLKS
#define CACHEBLKS		4095  	// # of memory blocks used for disk cache
#undef  CBLKSIZE
#define CBLKSIZE		2		// Default to 2k blocks (see above)
#undef  NMOUNTS
#define NMOUNTS			NBDEVS	// Max number of mounted file systems
								// Including NFS client. NFS client has
								// Similar adjustment in its info file
#undef  NGROUPS
#define NGROUPS			8		// Max number of supplementry groups
#undef  MAXSYMLINKS
#define MAXSYMLINKS		4		// Max number of symbolic links in a path

// Scheduling related.
#undef  QUANTUM
#define QUANTUM			25		// Clock ticks until preemption

/*
	Number of I/O interrupt handlers.
	Must be greater than 0.
	Must be in the range [1, 131072] for x86.
*/
#undef  NUMIOINTS
#define NUMIOINTS		80

/*
	Number of I/O interrupt vectors.
	Must be greater than OS ticker interrupt vector.
	Must be in the range [35, 256] for x86.
*/
#define NUMIOVECS		256

/*
	Number of kernel timers for VM0.
	Specifies the number of struct tout_entry allocated to tout_table.
*/
#undef  NUMTOUTS
#define NUMTOUTS        (VMZERO_NTHREADS)

/*
   System adjustables for VM0
   NPIPES    is number of struct fifo allocated to fifo_table
   NSOCKETS  is number of struct socket allocated to socket_table (these
             are UNIX domain stream sockets only).
   NRLOCKS   is number of struct locklist allocated to locklist (lockf())
 */

#undef  NPIPES
#define NPIPES			32		/* max number of pipes */
#undef  NSOCKETS
#define NSOCKETS		100		/* max number of sockets */
#undef  NRLOCKS
#define NRLOCKS			140		/* max number of lockf records */

/*
   POSIX.1 related things

   Except for the global parameter 'SIGQUEUE_MAX', the following values are
   VM0 limits, all other VM's have these values obtained from the VCT.

   MAX_FAST_SEMS   is number of struct fast_sem_entry in Fast_sem_table,
                   integral pages to hold this many struct fa_sem.
                   (POSIX semaphores, POSIX message queues use use 4 each)
   NUM_NAMES       is number of struct name_entry in Name_table
                   (POSIX semaphores, shared memory objects, and message
                    queues exist in this name space)
   NAME_BUF_SIZE   string table size for the name server.
   NUM_NAME_OPENS  is the number of struct open_entry in Open_table. This is
                   the max number of simultaneous name server objects open
                   at one time.
   NSHM            Max number of POSIX shared memory objects and the number of
                   struct shm_entry in Shm_table.
   NSIGNOTES       the number of struct signote in signote_table. Each
                   signotify() uses one of these
   SIGQUEUE_MAX    maximum number of queued signals we'll let a thread
                   accumulate
 */

#undef  MAX_FAST_SEMS
#define MAX_FAST_SEMS	((VMZERO_SEMAPHORES + VMZERO_MSGQS) * 3)
#undef  NUM_NAMES
#define NUM_NAMES		(VMZERO_SEMAPHORES + (2 * VMZERO_MSGQS) + NSHM)
#undef  NAME_BUF_SIZE
#define NAME_BUF_SIZE	(AVG_NAME_LEN * NUM_NAMES)
#undef  NUM_NAME_OPENS
#define NUM_NAME_OPENS	NUM_NAMES + (NUM_NAMES / 2)
#undef  NSHM
#define NSHM			VMZERO_NPROC + 6
/*signotes can have up to 3 events per msg q */
#undef  NSIGNOTES
#define NSIGNOTES		VMZERO_MSGQS * 3
#undef  SIGQUEUE_MAX
#define SIGQUEUE_MAX	256

/* End of process limits and things which cause memory allocation         */
/**************************************************************************/
/* Options of sorts                                                       */

#undef  WRITE_THROUGH
#define WRITE_THROUGH			0	/* enable write through cache (1=yes) */
#undef  REBOOT_ON_PANIC
#define	REBOOT_ON_PANIC			0	/* reboot on sys panic (1=yes) */

/*
 * Allow Health Monitor driver thread to write fault messages to the console
 * device (1=yes).  Note that since console printing is a waiting operation,
 * this may be undesired.  Normally, logging should be done by a Health
 * Monitor Application running in user mode in the System VM. This option
 * should only be turned on for debugging.
 */
#define HM_CONSOLE              1   /* HM driver prints to console (1=yes) */

/*
 * Allow Health Monitor to intercept fault signals such as SIGSEGV and
 * process them according to the actions specified in the VCT. If turned
 * off, the behavior is Unix-like, that is, fault signals are delivered
 * to the offending process even if the process has not registered a handler
 * for them.
 * This option should only be turned off during the development cycle.
 * In Production mode, Health Monitor always intercepts fault signals.
 */
#define HM_CATCH_FAULTS         0   /* HM catches fault signals (1=yes) */

/*
	Logging syslog() messages

	0 -	standart behaviour: syslog() messages will be sent using syslog
		protocol (udp/514)

	1 -	logging syslog() messages as Health Monitor events
*/
#define SYSLOG_AS_HM_EVENTS		0		// syslog() logs messages as HM events

#define PCI_MSI					true	// Enable or disable MSI/MSI-X

//#if (BUILD_MODE == DEVELOPMENT)
// SKDB configuration.
#define INSTALL_SKDB	1			// Install SKDB (1=yes)

#define	SKDB_COM1		1
#define	SKDB_COM2		2
#define	SKDB_COM3		3
#define	SKDB_COM4		4
#define SKDB_COMA		SKDB_COM1
#define SKDB_COMB		SKDB_COM2
#define SKDB_COMC		SKDB_COM3
#define SKDB_COMD		SKDB_COM4
#define	SKDB_COM11		11
#define	SKDB_COM12		12
#define	SKDB_COM21		21
#define	SKDB_COM22		22

#define SKDB_PORT		SKDB_COM1	// Set to the desired SKDB port
#define KKPF_PORT		SKDB_COM1	// Set to the desired kkprintf port

#define SKDB_KEY_SERIAL	('_'-'@')	// ctrl-'_'

// Must agree with SKDB_KEY_SERIAL to use TotalView -k.
#define SKDB_BRK_CHAR	SKDB_KEY_SERIAL
// endif // (BUILD_MODE == DEVELOPMENT)

/*
 * International Standard ISO/IEC 9945-1: 1996 (E)
 * IEEE Std 1003.1, 1996 Edition
 * (Incorporating ANSI/IEEE Stds 1003.1-1990,
 * 1003.1b-1993, 1003.1c-1995, and 1003.1i-1995)
 * Section 3.3.1.3 Signal Actions :
 *
 * b) If the SA_SIGINFO flag for the signal is cleared,
 *    the signal-catching function shall be entered as a
 *    C language function call as follows:
 *    void func(int signo);
 *    If the SA_SIGINFO flag for the signal is set,
 *    the signal-catching function shall be entered as a
 *    C language function call as follows:
 *    void func(int signo, siginfo_t *info, void *context);
 *
 * Section 3.3.1.2, Signal Generation and Delivery, requires that
 * in some situations, signals generated with the SA_SIGINFO flag
 * set must be queued to the process. The queuing of non-realtime
 * signals is an implementation defined behavior. LynxOS does not
 * support the queuing of non-realtime signals, and for this reason,
 * does not support the use of SA_SIGINFO with non-realtime signals.
 *
 * This behavior is rarely an issue for POSIX applications, LynuxWorks
 * has provided POSIX extensions so that applications that want to
 * access and modify the thread context may do so.
 *
 * Some Ada and JAVA runtimes depend on having access to the thread
 * context and pass the SA_SIGINFO flag for both realtime and
 * non-realtime signals. Rather than forcing them to modify their
 * code, we provide the following flag to provide the functionality
 * that they need.
 *
 * The NONPOSIX_SA_HANDLER_PROTO macro makes it possible
 * to provide signal-catching function with 'info' and
 * 'context' input parameters even if SA_SIGINFO flag is
 * not set or it is set for a non-realtime signal.
 *
 * It also allows signal-catching function to update
 * thread context even if SA_UPDATECTX flag is not set.
 *
 * The default value is 0, signal delivery to be compliant
 * with above-mentioned P1003.1 Section 3.3.1.3(b).
 *
 */
#define NONPOSIX_SA_HANDLER_PROTO (0)

/*
	Prior to the 2023.10 release, the behavior was to return -1
	and set errno to EINTR when clock_nanosleep() was interrupted by a signal.
	This was not the correct POSIX behavior. Starting with the 2023.10 release,
	clock_nanosleep() was corrected to return EINTR when interrupted.
	Setting NONPOSIX_CLOCK_NANOSLEEP to true causes the old behavior to
	continue, whereas setting it to false causes the correct POSIX behavior.
	This flag allows applications that expect the old non-POSIX behavior to
	continue to function correctly.
*/
#define NONPOSIX_CLOCK_NANOSLEEP false

/* thread kernel stack size, must be multiple of PAGESIZE */
#define KSTACKSIZE	(32U * PAGESIZE)

/* kernel stacks guard zone size, must be multiple of PAGESIZE */
#define KGUARDSIZE	(1U * PAGESIZE)

/* kernel stack low watermark limit for signal delivery */
#define KSTACK_SIG_DELIVERY_LWM ((5U * PAGESIZE) + (PAGESIZE / 2U))

#endif // !defined(__uparam_h)

// End of file.
