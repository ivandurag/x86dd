/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
/****************************************************************************
    (C) Copyright 2019-2022 Lynx Software Technologies, Inc. All rights reserved.
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

// Default GW entry
#define VM0_DEFAULT_GW \
{ \
	.rt_dst = {{{ 0x20,0x01,0x0d,0xb8,0x46,0x72,0x65,0x65, \
					0x20,0x26,0x50,0x44,0x2d,0x44,0x53,0x46 }}},\
	.rt_mask = {{{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }}},\
	.rt_gateway = {{{ 0x20,0x01,0x0d,0xb8,0x46,0x72,0x65,0x65, \
					0x20,0x26,0x50,0x44,0x2d,0x44,0x53,0x46 }}},\
	.rt_type = RT_HOST,\
	.rt_if = NULL \
}

	/* VM 0 LCS configuration */
lcs_config_blob_t config_blob = {
	.magic = LCS_CONFIG_BLOB_MAGIC,
	.version = LCS_CONFIG_BLOB_VERSION,
	.numOfVms = 1,
	{
		// VM0 LCS configuration
		{
			/* AF_INET/AF_INET6 sockets limit within this VM */
			.numOfSockets = 256,
			/* default RX buffer size, adjustable by SO_RCVBUF */
			.sockDefaultRxBufSize = 128 * 1024,
			/* default TX buffer size, adjustable by SO_SNDBUF */
			.sockDefaultTxBufSize = 128 * 1024,
			/* number of interfaces configured below */
			.numOfInterfaces = 3,
			/* Set to false to disable recv() polling */
			.recvPollingEnabled = true,
			/* IP/IP6 fragmentation queues number */
			.ipFragQueueSize = IP_FQSIZE,
			/* Wait up to 5 seconds for pending fragments to arrive */
			.ipFragWaitForTimeout = 5,
			/* Default TTL/hop number */
			.ipDefaultTTL = 64,
			/* Enable IP6 forwarding */
			.ipForwardingEnabled = true,
			/* number of multicast entries */
			.numOfMcastEntries = 2048,
			/* Static default GW */
			.ipv6StaticGW = VM0_DEFAULT_GW,
			{
				/* resources are not allocated until interface is attached to a driver */
				/* lcs0 interface configuration (driver-agnostic) */
				{
					/*
						The number of TX/RX ring depends on the Ethernet
						Controller. Some controllers support more than one ring:
						- enetc part of NXP LS1028A SoC supports 8 TX/RX queues
						- i225 of Intel supports 4 TX/RX queues
	
					*/
					.numOfTxRings = 1,
					.numOfRxRings = 1,
					/*
						The maximum TX/RX ring size depends on the Ethernet
						Controller. Please reference lcs_user_guide for maximum
						limit of ixgbe TX/RX ring size.
					*/
					.txRingSize = 256,
					.rxRingSize = 256,
					/*
						VLAN by default is disabled by setting vlan_count = 0.
						Any value larger than 0 will enable this feature.
						It should be enabled only on
						Ethernet controllers that support it. The Ethernet
						controllers that support VLAN:
							- enetc part of NXP LS1028A SoC.
	
						Example of one VLAN entry, to add more adjust vlan_count
						and add more entries to vlan_list.
						Each VLAN will show up as a separate interface.
						LCS supports maximum 8 interfaces where one is allocted to
						the loopback interface.
					*/
					.vlan_count = 0,
					.vlan_list = {
						{
							.vlan_id = 10,
							.vlan_prio = TC_3
						}
					},
					.selfAddr6 = IP6_SOCKADDR_INIT(
							0x20, 0x01, 0x0d, 0xb8,
							0x46, 0x72, 0x65, 0x65,
							0x20, 0x26, 0x50, 0x43,
							0x2d, 0x42, 0x53, 0x47),
	
					/*
						Specifies the number of RX descriptors to be processed by
						LCS thread per polling period per interface per ring.
						For every iteration over numOfRxRings, specified in this
						file, LCS will dedicate half of the rx_budget to the
						loopback interface and the other half to the RX ring.
						In essense each RX ring will have (rx_budget / 2)
						descriptors processed.
					*/
					.rx_budget = 256,
					/*
						Specifies the number of TX decriptors to be processed by
						LCS thread per polling period per interface.
						For interfaces that support more than one queue with
						different priorities the budget starts from highest
						priority queue until this queue gets exhausted it
						continues to the lower priority queues, till the
						budget is exhausted.
					*/
					.tx_budget = 256,
					/*
						When the number of queued packets in the stack reaches this
						threshold the packets will be transmitted.
						To transmit a packet immediately, this count needs to be
						zero.
					*/
					.txThreshold = 32,
					/*
						Currently LCS supports 8 TCs, with a driver like enetc
						with 8 TX/RX queues each ring needs 256, for RX total
						of 8 * 256 = 2048, same for TX 2048.
						Without this some of the rings won't have buffer
						descriptors.
					*/
					.perIfIorbPoolSize = 4096,
					/*
						All Ethernet Controllers support standard MTU size of 1500.
						It can be changed to jumbo only on controllers that
						support it, such as:
						- Intel ixgbe
	
					*/
					.mtu = LCS_MTU_MAX,
	
					/*
						LCS uses this table as a cache to list the known remote
						peers this interface needs to communicate to.
						It uses pairs of IP/MAC address.
						Adding a new pair requires bumping up numOfPeers.
					*/
					.numOfPeers  = 2,
					.ip2macPeers = {
						{
							.peerIpAddr = IP6_SOCKADDR_INIT(
								0x20, 0x01, 0x0d, 0xb8,
								0x46, 0x72, 0x65, 0x65,
								0x20, 0x26, 0x50, 0x44,
								0x2d, 0x43, 0x53, 0x48),
							.peerMacAddr = { 0xa0, 0x36, 0x9f, 0x7f, 0x17, 0xf4 }
						},
						{
							.peerIpAddr = IP6_SOCKADDR_INIT(0x20, 0x01, 0x0d, 0xb8, 0x46, 0x72, 0x65, 0x65, 0x20, 0x26, 0x50, 0x44, 0x2d, 0x44, 0x53, 0x46),
							.peerMacAddr = { 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed }
						}
					},
					.ieee_802_1_Q_sched_parameters = {
						/*
							802_1_Q configuration used by the Ethernet Controller.
							This table should only be used on NXP LS1028A Ethernet
							Controller which supports scheduled parameters that
							have gate control list of different traffic classes.
							The below fields are defined in lcs_802_1_Q_config.h
							and based on the YANG data model by IEEE.
							The default is no QBV:
							gate_enabled = false
							All the configuration parameters after it will be
							ignored. To activate QBV this flag needs to be true.
							gate_states_value is 8 bit mask corresponding to 8 TCs.
							Bit 0 is TC0, Bit 7 is TC7.
						*/
						.gate_enabled = false,
						.admin_control_list_length = 3,
						.admin_base_time = {
							.seconds = 1,
							.fractional_seconds = 1000
						},
						.admin_cycle_time = {
							.numerator = 100,
							.denominator = 1000000
						},
						.admin_control_list = {
							{
								.gate_states_value = 0x1,
								.time_interval_value = 25000},
							{
								.gate_states_value = 0x2,
								.time_interval_value = 25000},
							{
								.gate_states_value = 0x4,
								.time_interval_value = 50000},
						}
					}
	                /*
						pf2vf_maclist entry should only be used on IXGBE Intel
						Ethernet Controller.
	                    How to turn on SRIOV and use pf2vf_maclist entry.
	                    In ixgbepfinfo.c look for these lines: PORT_INFO(true, 0, 0, false),
	                    each one enables PF port in the HW, "true" means enabled".
	                    The second paramater which is zero is the num_vfs number of VFs to expose
	                    in SRIOV. Since it is 0 means SRIOV is disabled by default.
	                    To enable SRIOV :
	
	                    1) Change num_vf in ixgbepfinfo.c to value between 1,31 inclusive.
	                    2) Supply your list of VF MAC address list using .pf2vf_maclist and should be equal to
	                       number of num_vfs, if not PF driver will fail.
	                    3) Make this interface the PF interface.
	                       Look at VCT.in file and search for ixgbepf0 and enable it.
	                       If you assigned VFs to same subject enable the VFs as listed.
	                       If you have two PFs then each needs such a list and be specified in VCT.in.
	                       If your setup requires multiple KDIs then use blobs and look in lcs_user_guide for this topic.
	                    4) All references to VF macAddr in lcs interface of those VFs will be
	                       ignored by the VF driver as the PF driver will have all the repository of
	                       VF Mac addresses.
	                    5) PF driver will fail for zero MAC address or duplicate in .pf2vf_maclist
	                       Make sure you supply MAC addresses equal to num_vfs else the PF driver
	                       will regard the missing ones as zero and fail.
	                    6) The PF driver will fail for duplicate mac address between .pf2vf_maclist
	                       and .macAddr
	                    7) The PF mac address is not part of pf2vf_maclist, instead it uses the entry
	                       .macAddr below.
	                    8) When building the SRP don't use any VF above 31 listed in the HW database file,
	                       like NET0#32,..., NET0#63 PF driver supports the first 31.
	
	                    Example:
	                    List below is provided as an example for the case num_vfs=9,
	                    and place holder, user can change it.
	
	                    The VFs are mapped to list below as follows:
	                    VFn <--> NET0#n <--> row n+1 in the list
	                    VF0 <--> NET0#0 <--> {0x00,0x15,0x90,0x09,0x28,0xa0}
	                    ....
	                    VF8 <--> NET0#8 <--> {0x00,0x15,0x90,0x09,0x28,0xa8}
	
	                .pf2vf_maclist = {
	                    {0x00,0x15,0x90,0x09,0x28,0xa0},
	                    {0x00,0x15,0x90,0x09,0x28,0xa1},
	                    {0x00,0x15,0x90,0x09,0x28,0xa2},
	                    {0x00,0x15,0x90,0x09,0x28,0xa3},
	                    {0x00,0x15,0x90,0x09,0x28,0xa4},
	                    {0x00,0x15,0x90,0x09,0x28,0xa5},
	                    {0x00,0x15,0x90,0x09,0x28,0xa6},
	                    {0x00,0x15,0x90,0x09,0x28,0xa7},
	                    {0x00,0x15,0x90,0x09,0x28,0xa8}
	                },
	                */
					// This overrides the HW MAC address.
					//.macAddr = {0x02, 0x00, 0x0a, 0x0b, 0x0c, 0x00 },
				},
				/* lcs1 interface configuration */
				{
					.numOfTxRings = 1,
					.numOfRxRings = 1,
					// Please reference lcs_user_guide for maximum limit of ixgbe TX/RX ring size
					.txRingSize = 256,
					.rxRingSize = 256,
					.rx_budget = 256,
					.tx_budget = 256,
					.txThreshold = 32,
					// recommended value is not less than rxRingSize * 4
					.perIfIorbPoolSize = 2048,
					.mtu = LCS_MTU_MAX,
				},
				/* lcs2 interface configuration */
				{
					.numOfTxRings = 1,
					.numOfRxRings = 1,
					// Please reference lcs_user_guide for maximum limit of ixgbe TX/RX ring size
					.txRingSize = 256,
					.rxRingSize = 256,
					.rx_budget = 256,
					.tx_budget = 256,
					.txThreshold = 32,
					// recommended value is not less than rxRingSize * 4
					.perIfIorbPoolSize = 2048,
					.mtu = LCS_MTU_MAX,
					.macAddr = {0x02, 0x00, 0x0a, 0x0b, 0x0c, 0x02 }
				},
			/* and so on */
			{ }
		},
		/* Put VM 1...15 LCS configuration here one by one */
		{ }
	}
};

// End of file.
