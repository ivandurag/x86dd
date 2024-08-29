/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
#if !defined(__lcsdrvr_h)
#define __lcsdrvr_h
/****************************************************************************
(C) Copyright 2019-2020 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

// LCS includes.
#include <lcs/lcs_sock.h>
#include <lcs/lcs_private.h> // lcs_stat_t

// Debugging support.
#define DEBUG
#undef DEBUG

extern int lcs_start_thread(lcs_stat_t *);
#endif // !defined(__lcsdrvr_h)

// End of file.
