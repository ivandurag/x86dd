/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
/****************************************************************************
(C) Copyright 2019-2024 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

// Implementation of LCS per-VM thread.

// User includes.
#include <debugdef.h>		// VMOS_ASSERT()

// Common kernel includes.
#include <kthread.h>		// kthread_attr_t, ...
#include <kprintf.h>		// ksprintf()
#include <timeval2ticks.h>
// LCS includes.
#include <lcs/system.h>
#include <lcs/iorb.h>
#include <lcs/lcs_sock.h>
#include <lcs/lcs_private.h>	// lcs_stat_t
#include <lcs/lld.h>			// LldProcessRx()
#include <lcs/lcs_ip.h>			// lcsCheckReassmTimeout()

/********************************* DEFINES  *********************************/

//TODO: move the following to lcs_gonfig.h
// LCS per VM thread default priority
#define LCS_DEFAULT_PRIO 16

// LCS per-VM thread default stack size
#define LCS_DEFAULT_KSTACK (128*(KB))

// LCS poll timer default resolution
#define LCS_POLL_TIMER_RES (100 * 1000) // ms

/****************************************************************************/

/*
	LCS per-VM thread function.
*/
static int
lcs_VM_thread(lcs_stat_t *lcs)
{
	int i;
	struct IfPhy *pIf;
	unsigned int lcs_pending_ops, if_pending_ops;
	os_mode_t osm;
#if (BUILD_MODE == DEVELOPMENT)
	unsigned long uptime;
	int timeout;

	uptime = ticks_since_boot;
#endif
	for (;;) {
		// no timers in PDN mode, so this code is not needed
#if (BUILD_MODE == DEVELOPMENT) // No periodic wakeup in PDN
		timeout = timeval2ticks(0, LCS_POLL_TIMER_RES, 0)
				- uptime % timeval2ticks(0, LCS_POLL_TIMER_RES, 0);
		// Wait for event
		if (tswait(&lcs->event_sem, SEM_SIGIGNORE, timeout)
				== TSWAIT_TIMEDOUT) {
			// Force poll in DEV mode
			lcs->pending_ops |= LLD_SCHED_RX;
		}

		uptime = ticks_since_boot;

		lcsTimersCheck();
#else
		/*
			This thread can be waken up by recv() operation only when
			built in PDN mode.
		*/
		swait(&lcs->event_sem, SEM_SIGIGNORE);
#endif /* BUILD_MODE */

		// Handle IP/IP6 reassemblies
		(void)lcsCheckReassmTimeout();

		// May change during RX/TX, so save it here
		__osm_intr_disable(&osm);
		lcs_pending_ops = lcs->pending_ops;
		lcs->pending_ops &= ~LLD_SCHED_BOTH;
		__osm_intr_restore(&osm);

		// Process RX and TX
		for (i = 0; i < LLD_NUM_TRUNKS; i ++) {
			pIf = &lcs->if_trunk_array[i];

			// First of all, check if interface was marked as faulty.
			// And try to recover by stopping and re-starting the interface.
			if (pIf->IfState & LLD_FAULTY) {
				pIf->net_ops->stop(pIf);
				pIf->net_ops->start(pIf);
				pIf->IfState &= ~LLD_FAULTY;
			}
			// May change during RX/TX, so save it here
			__osm_intr_disable(&osm);
			if_pending_ops = pIf->pending_ops | lcs_pending_ops;
			pIf->pending_ops &= ~LLD_SCHED_BOTH;
			__osm_intr_restore(&osm);

			if (if_pending_ops & LLD_SCHED_RX) {
				LldProcessRx(pIf);
			}

			if (if_pending_ops & LLD_SCHED_TX) {
				LldProcessTx(pIf);
			}
		}
	}

	// Unreachable
	return 0;
}

int
lcs_start_thread(lcs_stat_t *lcs)
{
	kthread_attr_t attr;
	int res;
	char thread_name[9];
	tid_t tid;

	ksprintf(thread_name, "LCS_VM%02d", curr_vm);
	lcs->curr_prio = KTHREAD_PRIO(LCS_DEFAULT_PRIO);

	kthread_attr_init(&attr);
	kthread_attr_setstacksize(&attr, LCS_DEFAULT_KSTACK);
	kthread_attr_setvmid(&attr, curr_vm);
	kthread_attr_setprio(&attr, lcs->curr_prio);
	kthread_attr_setname(&attr, thread_name);

	res = kthread_create(&tid, &attr,
			(void *(*)(void *))&lcs_VM_thread, lcs);

	if (res == SYSERR) {
		return res;
	}

	debug(("LCS thread %d started in VM %d\n", tid, curr_vm));
	(void) priot_add(&lcs->priot, LCS_DEFAULT_PRIO);

	return (int)tid;
}

// End of file.
