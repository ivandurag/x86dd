// vi: set ai ts=4 sw=4 : -*- tab-width:4 c-basic-offset:4 -*-
// vim: set comments= cinoptions=\:0,t0,+8,c4,C1 :
/****************************************************************************
(C) Copyright 2021-2024 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

#include <lcs/lcs_private.h>
#include <lcs/lcs_sock.h>
#include <lcs/lld.h>
#include <lcs/lcs_ip.h>
#include <clock_impl.h>		// clock_table[]
#include <timex.h>	// struct timex
#include <errno.h>	// errno
#include <stdlib.h>	// free()
#include <atomic_ops.h>	// ATOMIC_INC
#include <debugdef.h>

// Next available PHC clock id to use,
// should be shared across all VMs.
static int next_available_phc_clock_id = CLOCK_PTP;

static inline int allocate_and_test_clock_id(void)
{
	int id = -1;
	os_mode_t osm;

	__osm_ctxsw_disable(&osm);
	if (next_available_phc_clock_id <= CLOCK_PTP_MAX) {
		id = next_available_phc_clock_id;
		ATOMIC_INC(next_available_phc_clock_id);
	}
	__osm_ctxsw_restore(&osm);
	return id;
}

static int
lcs_ptp_settime(struct clock_descr *clk_ptr, struct timespec *time)
{
	struct IfPhy *pIf = clk_ptr->driver_data;

	pIf->ptp_ops->settime(pIf, time);
	return OK;
}

static int
lcs_ptp_gettime(struct clock_descr *clk_ptr, struct timespec *time)
{
	struct IfPhy *pIf = clk_ptr->driver_data;

	pIf->ptp_ops->gettime(pIf, time);
	return OK;
}

static int
lcs_ptp_adjtimex(struct clock_descr *clk_ptr, timex_t *tx) {
	struct IfPhy *pIf;
	struct timespec ts;
	int rv = OK;

	pIf = clk_ptr->driver_data;

	if ((tx->atx_modes & MODE_SETOFFSET) != 0) {
		__cvt_ns_to_ts(tx->atx_time, ts);

		if ((tx->atx_modes & MODE_NANO) == 0) {
			ts.tv_nsec *= 1000;
		}
		pIf->ptp_ops->adjtime(pIf, &ts);
	} else if ((tx->atx_modes & MODE_FREQUENCY) != 0) {

		pIf->ptp_ops->adjfreq(pIf, tx->atx_freq);
		// preserve accumulated frequency correction to pick up
		// from where we left in case of a servo restart.
		pIf->ptp_servo_correction = tx->atx_freq;
	} else if (tx->atx_modes == 0) {
		tx->atx_freq = pIf->ptp_servo_correction;
	} // No final else required
	return rv;
}

int ptp_clock_register(struct IfPhy *pIf)
{
	int rv = SYSERR;
	struct clock_descr *clk_ptr;
	int clock_id;

#if (BUILD_MODE == DEVELOPMENT)
	if ((pIf->ptp_ops == NULL) || (pIf->ptp_ops->settime == NULL)
			|| (pIf->ptp_ops->gettime == NULL)
			|| (pIf->ptp_ops->adjtime == NULL)
			|| (pIf->ptp_ops->adjfreq == NULL)) {
		errno = EINVAL;
	} else
#endif // BUILD_MODE == DEVELOPMENT
	if ((clock_id = allocate_and_test_clock_id()) == -1) {
		errno = ENOSPC;
	} else if ((clk_ptr = calloc(1, sizeof(struct clock_descr))) != NULL) {
		clk_ptr->clock_id = clock_id;
		clk_ptr->clock_res = 1;
		clk_ptr->clock_settime = lcs_ptp_settime;
		clk_ptr->clock_gettime = lcs_ptp_gettime;
		clk_ptr->clock_adjtimex = lcs_ptp_adjtimex;
		clk_ptr->clock_activate = ionull;
		clk_ptr->driver_data = pIf;

		if (clock_register(clk_ptr) != NULL) {
			pIf->ptp_clock_id = clock_id;
			rv = OK;
		} else { // clock_register() sets errno on failure
			free(clk_ptr);
		}
	} // calloc() sets errno on failure
	return rv;
}
