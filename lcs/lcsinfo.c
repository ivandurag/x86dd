/* Automatically generated file. Do not edit!*/
/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
/****************************************************************************
(C) Copyright 2019-2023 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/


#include <lcs/lcs_config.h>
#include <vmosschedu.h>
#include <lcs/system.h>
#include <lcs/arm.h>
#include <lcs/ip6_rtentry.h>
#include <lcs/rt_types.h>
#include <lcs/lcs_ip.h> // IP_*
#include <lcs/lld.h> // LCS_MTU_MAX
#include <lcs/iorb.h> // SOCK_IORB_COUNT

lcs_init_args_t lcs_config[NUM_VMS] = {
	{
		.numOfSockets = 256,
		.sockDefaultRxBufSize = 128 * 1024,
		.sockDefaultTxBufSize = 128 * 1024,
		.numOfInterfaces = 1,
		.recvPollingEnabled = true,
		.ipFragQueueSize = 1024,
		.ipFragWaitForTimeout= 5,
		.ipDefaultTTL = 64,
		.ipForwardingEnabled = true,
		.numOfMcastEntries = 2048,
		.ipv6StaticGW = {
			.rt_dst = {{{  0x20, 0x01, 0x0d, 0xb8, 0x46, 0x72, 0x65, 0x65, 0x20, 0x36, 0x50, 0x44, 0x2d, 0x44, 0x53, 0x46 }}},
			.rt_mask = {{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}},
			.rt_gateway = {{{  0x20, 0x01, 0x0d, 0xb8, 0x46, 0x72, 0x65, 0x65, 0x20, 0x36, 0x50, 0x44, 0x2d, 0x44, 0x53, 0x46 }}},
			.rt_type = RT_HOST,
			.rt_if = NULL,
		},
		{
			{
				.numOfTxRings = 1,
				.txRingSize = 256,
				.numOfRxRings = 1,
				.rxRingSize = 256,
				.vlan_count = 0,
				.mtu = 1500,
				.rx_budget = 256,
				.tx_budget = 256,
				.txThreshold = 32,
				.perIfIorbPoolSize = 4096,
				.numOfVfs = 0,
				.numOfPeers = 1,
				.ip2macPeers = {
					{
						.peerIpAddr = IP6_SOCKADDR_INIT( 0x20, 0x01, 0x0d, 0xb8, 0x46, 0x72, 0x65, 0x65, 0x20, 0x36, 0x50, 0x44, 0x2d, 0x44, 0x53, 0x46, (64)),
						.peerMacAddr = { 0xde,0xad,0xbe,0xef,0xfe,0xed }
					},
				},
				.ieee_802_1_Q_sched_parameters = {
					.gate_enabled = false
				},
			},
		}
	},
};
