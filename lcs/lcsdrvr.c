/* vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*- */
/* vim: set comments= cinoptions=\:0,t0,+8,c4,C1 : */
/****************************************************************************
   (C) Copyright 2019-2024 Lynx Software Technologies, Inc. All rights reserved.
****************************************************************************/

// Implementation of driver interface for LCS module.

#include <kernel.h>
#include <kern_proto.h>
#include <errno.h>
#include <string.h>

#include <lcs/lcs_ip.h>
#include <lcs/lcs_sock.h>
#include <lcs/lcs_stats.h>
#include "lcsdrvr.h"

#include <lcs/lcs_config.h>
#include <lcs/lcs_ioctl.h>
#include <lcs/arm.h>
#include <lcs/lcs.h>
#include <lcs/lcs_init.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <debugdef.h>
#if (BUILD_MODE == DEVELOPMENT)
#include <kprintf.h>		// kkprintf()
#endif

#if defined(DLDD)
#include <dldd.h>
#endif
#include <sys/cdefs.h>		// ATTR_USED
#include <netinet/in.h>

#include <lcs/system.h>

extern int LldInit(lcs_stat_t *lcs, const lcs_init_args_t *args);

// Define LCS initializer function type.
typedef int (*init_t)(lcs_stat_t *, const lcs_init_args_t *);

lcs_stat_t lcs_data[NUM_VMS];

static lcs_init_args_t *lcs_init_args;

// LCS statistics.
struct Tcpip_lcs_info Tcpip_lcs_info[NUM_VMS];

/******************************************************************************
    Name: lcs_info_init

    Description:

    Initialization of info structure used by lcsnetstat.
    This is going to be called during the KLCS_INIT ioctl() as far as
    this data are per-VM.

    Input: Valid VM number.

    Returns: None.
******************************************************************************/
ATTR_NOINLINE ATTR_USED static void
lcs_info_init(int VMid)
{
	Tcpip_lcs_info[VMid].ip6stat = &lcs_data[VMid].ip6stat;
	Tcpip_lcs_info[VMid].udp6stat = &lcs_data[VMid].udp6stat;
	Tcpip_lcs_info[VMid].icmp6stats = &lcs_data[VMid].icmp6stats;

	Tcpip_lcs_info[VMid].socktab = lcs_data[VMid].st->entries;

	Tcpip_lcs_info[VMid].ifstab = lcs_data[VMid].if_trunk_array;
	Tcpip_lcs_info[VMid].rtable6 = lcs_data[VMid].in6_routing_table;

	Tcpip_lcs_info[VMid].nsocks = lcs_data[VMid].st->nsocks;
	Tcpip_lcs_info[VMid].rtable_cnt6 = NCS_MAX_ROUTES;
	Tcpip_lcs_info[VMid].ifphy_cnt = LLD_NUM_TRUNKS;
#if (BUILD_MODE == DEVELOPMENT)
	Tcpip_lcs_info[VMid].ipstat = &lcs_data[VMid].ipstat;
	Tcpip_lcs_info[VMid].udpstat = &lcs_data[VMid].udpstat;
	Tcpip_lcs_info[VMid].icmpstat = &lcs_data[VMid].icmpstat;
	Tcpip_lcs_info[VMid].tcpcb = lcs_data[VMid].tcpcb;
	Tcpip_lcs_info[VMid].tcpstat = &lcs_data[VMid].tcpstat;
	Tcpip_lcs_info[VMid].arptab = lcs_data[VMid].ArpTableArray;
	Tcpip_lcs_info[VMid].arptable_cnt = MAX_ARP_ENTRIES;
	Tcpip_lcs_info[VMid].rtable = lcs_data[VMid].in_routing_table;
	Tcpip_lcs_info[VMid].rtable_cnt = NCS_MAX_ROUTES;
#endif
}

/******************************************************************************
    Name: lcs_install

    Description:

    The install entry point.
    Install the LCS operations into the kernel.

    Input: lcs_info structure pointer

    Returns: NULL
******************************************************************************/
void *
lcs_install(void *info)
{
	nux = &lcsnux_sockops;

	lcs_init_args = info;

	// Install the LCS statistics pointer
	Tcpip_info = Tcpip_lcs_info;

	// The module is not using statics
	return NULL;
}

/******************************************************************************
    Name: lcs_uninstall

    Description:
    The uninstall routine is called when the driver is uninstalled.
    This call surves debug purpose only.

    Input: lcs_stat_t structure pointer

    Returns: OK
******************************************************************************/
int
lcs_uninstall(lcs_stat_t *unused)
{
	debug(("%s: uninstall\n", __FUNCTION__));
	seterr(ENOSYS);
	return SYSERR;
}

/******************************************************************************
    Name: lcs_allocate_st

    Description: Allocate socket table and socket entries in the table for the
    current VM.

    Input: nsocks - number of sockets.

    Returns: Pointer to allocated socket table or NULL in case of failure.

******************************************************************************/
ATTR_NOINLINE ATTR_USED static lcs_stable_t *
lcs_allocate_st(unsigned int nsocks)
{
	lcs_sock_entry_t *se;
	lcs_stable_t *st;
	int i;

	debug(("%s: Allocating socket table\n", __FUNCTION__));

	st = (lcs_stable_t *)sysbrk(sizeof(lcs_stable_t));

	// Socket table allocation failed.
	if (st == NULL) {
		return NULL;
	}

	debug(("%s: Allocating %d socket entries\n", __FUNCTION__, nsocks));

	// Allocate the socket entries
	st->nsocks = nsocks;
	st->entries = (lcs_sock_entry_t *)
			sysbrk(nsocks*sizeof(lcs_sock_entry_t));

	// Socket entries allocation failed.
	if (st->entries == NULL) {
		return NULL;
	}

	// Initialize the socket entries
	memset((char *)st->entries, 0, nsocks*sizeof(lcs_sock_entry_t));
	// Point the head of free list to the first socket entry
	se = st->entries;
	SLIST_INIT(&st->free);
	for (i = 0; i < nsocks; i++) {
		se[i].sock_id = UINT_MAX;
		// Add socket entry to the free list
		SLIST_INSERT_HEAD(&st->free, &se[i], next);
	}
	debug(("%s: %d socket entries allocated\n", __FUNCTION__, nsocks));

	debug(("Socket entries from %08x\n", (kaddr_t)st->entries));
#if defined(DEBUG)
	SLIST_FOREACH(se, &st->free, next) {
		debug(("\t sock[%08x]: ID %d\n", se, se->sock_id));
	}
#endif
	return st;
}

#if (BUILD_MODE == DEVELOPMENT)
static inline void
lcs_set_defaults(lcs_init_args_t *args)
{
	if (0 == args->ipFragQueueSize) {
		args->ipFragQueueSize = IP_FQSIZE;
	}
	if (0 == args->ipFragWaitForTimeout) {
		args->ipFragWaitForTimeout = 1;
	}
	if (0 == args->ipDefaultTTL) {
		args->ipDefaultTTL = 64;
	}
}
#endif	// BUILD_MODE

ATTR_NOINLINE ATTR_USED static void
lcs_init(lcs_stat_t *lcs, lcs_init_args_t *args)
{
	int i;
	static const init_t initializer[] = {
		LldInit,
		ClientPortNumber_Initialize,
		ip_common_init,
		ip6_init,
		icmp6_init,
		ip4_init,
		icmp_init,
#if (BUILD_MODE == DEVELOPMENT)
		TimersInit,
		igmp_init,
		tcp_init,
#endif
		udp_init,
	};
#if (BUILD_MODE == DEVELOPMENT)
	lcs_set_defaults(args);
#endif
	for (i = 0; i < ARRAY_SIZE(initializer); i ++) {
#if (BUILD_MODE == DEVELOPMENT)
		int ret;

		debug(("Initializer[%d]\n", i));
		ret = (initializer[i])(lcs, args);
		VMOS_ASSERT(ret == 0, "cannot initialize LCS");
#else
		(void) (initializer[i])(lcs, args);
#endif
	}
}


/******************************************************************************
	Name: update_peer_entry

	Description:
	update_peer_entry add ipv6 peer entry into specific lcsx
	interface.
	[in]: IfPhy structure pointer
	[in]: ipandmac_t pointer contains user passed ipv6, mac addresses
	to be added as peer

	Returns: OK on success else SYSERR and set appropiate errno
******************************************************************************/
int32_t
update_peer_entry(struct IfPhy *pIf, ipandmac_t *data)
{
	int32_t i;
	int32_t idx;
	int32_t num_of_peers;
	struct sockaddr *p_sockaddr;
	char *peer_mac_addr;
	char entry_not_found[PEER_MAX];
	char anyFalse;
	ip2mac_entry_t *ip2macPeers = NULL;

	num_of_peers = pIf->IfConfig.numOfPeers;
	ip2macPeers = pIf->IfConfig.ip2macPeers;

	// Check if peer list is already full
	if (num_of_peers >= PEER_MAX) {
		memcpy(data->err_str, "Peer list is full.",
				sizeof(data->err_str));
		seterr(ENOMEM);
		return SYSERR;
	}

	if (ip2macPeers == NULL) {
		memcpy(data->err_str, "Device structure is not present.",
				sizeof(data->err_str));
		seterr(ENXIO);
		return SYSERR;
	}
	memset(entry_not_found, true, sizeof(entry_not_found));

	for (idx = 0; idx < num_of_peers; ++idx) {
		peer_mac_addr = &ip2macPeers[idx].peerMacAddr[0];
		p_sockaddr = &(ip2macPeers[idx].peerIpAddr);

		for (i = 0; i < IPV6_END_OFFSET; i++) {
			if ( p_sockaddr->sa_data[i + IPV6_START_OFFSET]
					!= data->ipv6_addr[i]) {
				entry_not_found[idx] = false;
				break;
			}
		}
	}

	anyFalse = false;
    for (i = 0; i < num_of_peers; i++) {
        if (entry_not_found[i] == true) {
            anyFalse = true;
            break;
        }
    }

	if (anyFalse == true) {
		memcpy(data->err_str, "IPV6 entry already present.",
				sizeof(data->err_str));
		seterr(EEXIST);
		return SYSERR;
	}

	peer_mac_addr = &ip2macPeers[num_of_peers].peerMacAddr[0];
	p_sockaddr = &(ip2macPeers[num_of_peers].peerIpAddr);

	p_sockaddr->sa_family = AF_INET6;
	p_sockaddr->sa_len = 16;
	for (i = 0; i < IPV6_END_OFFSET; i++) {
		p_sockaddr->sa_data[i + IPV6_START_OFFSET] = data->ipv6_addr[i];
	}
	p_sockaddr->sa_data[22] = data->prefix_length;

	for (i = 0; i< ETHER_ADDR_LEN; i++) {
		peer_mac_addr[i] = data->mac_addr[i];
	}

	pIf->IfConfig.numOfPeers++;
	return OK;
}

/******************************************************************************
	Name: delete_peer_entry

	Description:
	delete_peer_entry delete ipv6 peer entry from specific lcsx
	interface.
	[in]: IfPhy structure pointer
	[in]: ipandmac_t pointer contains user passed ipv6 address to
	delete

	Returns: OK on success else SYSERR and set appropiate errno
******************************************************************************/


int32_t
delete_peer_entry(struct IfPhy *pIf, ipandmac_t *data)
{
	int i;
	int idx;
	int num_of_peers;
	struct sockaddr *p_sockaddr;
	char *peer_mac_addr;
	char anyFalse;
	char entry_not_found[PEER_MAX];
	ip2mac_entry_t *ip2macPeers = NULL;


	num_of_peers = pIf->IfConfig.numOfPeers;

	ip2macPeers = pIf->IfConfig.ip2macPeers;
	if (ip2macPeers == NULL) {\
		memcpy(data->err_str, "Device structure is not present.",
				sizeof(data->err_str));
		seterr(ENXIO);
		return SYSERR;
	}
	debug(("num_of_peers=%d\n", num_of_peers));
	memset(entry_not_found, false, sizeof(entry_not_found));

	for (idx = 0; idx < num_of_peers; ++idx) {
		peer_mac_addr = &ip2macPeers[idx].peerMacAddr[0];
		p_sockaddr = &(ip2macPeers[idx].peerIpAddr);

		for (i = 0; i < IPV6_END_OFFSET; i++) {
			if ( p_sockaddr->sa_data[i + 6] != data->ipv6_addr[i]) {
				entry_not_found[idx] = true;
				break;
			}
		}
	}

	anyFalse = false;
	int entry_idx = 0;
    for (i = 0; i < num_of_peers; i++) {
        if (entry_not_found[i] == false) {
			entry_idx = i;
            anyFalse = true;
            break;
        }
    }

	if (anyFalse == true) {
		debug(("IPv6 entry found...removing...\n"));
		pIf->IfConfig.numOfPeers--;
		memcpy(&ip2macPeers[entry_idx], &ip2macPeers[entry_idx + 1],
				pIf->IfConfig.numOfPeers * sizeof(ip2mac_entry_t));
		return 0;
	} else {
		memcpy(data->err_str, "IPV6 entry not found.",
				sizeof(data->err_str));
		seterr(ENXIO);
		return SYSERR;
	}
	return 0;
}

/******************************************************************************

    Name: lcs_ioctl

    Description: The ioctl is called upon the application ioctl to the device

    Input: lcs_stat_t pointer, file structure pointer, command,
    argument to command.

    Returns: OK or SYSERR
******************************************************************************/
int
lcs_ioctl(lcs_stat_t *unused, struct file *f, int cmd, char *args)
{
	debug(("%s: ioctl entry point is called for command %x\n",
			__FUNCTION__, cmd));
	lcs_stat_t *we  = &lcs_data[curr_vm];
	lcs_init_args_t *config = &lcs_init_args[curr_vm];
	unsigned int *budgets;
	struct IfPhy *pIf;
	int i;
	int ret;
	ipandmac_t *data;

	switch (cmd) {
		case LCS_VM_INIT:
			// Perform LCS initialization within VM it has been called from
			debug(("%s: Initializing VM %d\n", __FUNCTION__, curr_vm));

			if (we->st != NULL) {
				seterr(EINVAL);
				return SYSERR;
			}
			memset(we, 0, sizeof(*we));
			// if LCS config blob was provided - it takes priority
			if (args) {
				if (rbounds((kaddr_t)args) < sizeof(lcs_init_args_t)) {
					seterr(EFAULT);
					return SYSERR;
				}
				// Cast first to void to avoid compiler warning on ARM
				config = (lcs_init_args_t *)(void *)args;
			}

			// Initialize the target VMs socket table
			we->st = lcs_allocate_st(config->numOfSockets);
			if (we->st == NULL) {
				seterr(ENOMEM);
				return SYSERR;
			}
			we->numOfSockets = config->numOfSockets;
			we->numOfMcastEntries = config->numOfMcastEntries;
			we->sockDefaultRxBufSize = PAGE_UPPER_ALIGN(config->sockDefaultRxBufSize);
			we->sockDefaultTxBufSize = PAGE_UPPER_ALIGN(config->sockDefaultTxBufSize);
			we->recvPollingEnabled = config->recvPollingEnabled;

			lcs_init(we, config);
			lcs_info_init(curr_vm);

			// Start service thread for current VM
			we->tid = lcs_start_thread(we);
			break;
		case LCS_SET_BUDGET:
			if (rbounds((kaddr_t)args) < LLD_NUM_TRUNKS
					* sizeof(unsigned int)) {
				seterr(EFAULT);
				return SYSERR;
			}
			if (we->st == NULL) {
				seterr(EINVAL);
				return SYSERR;
			}
			// Cast first to void to avoid compiler warning on ARM
			budgets = (unsigned int *)(void *)args;
			for (i = 0; i < LLD_NUM_TRUNKS; i ++) {
				pIf = &we->if_trunk_array[i];
				if (budgets[i]) {
					pIf->IfConfig.rx_budget = budgets[i];
				}
			}
			break;
		case LCS_GET_BUDGET:
			if (wbounds((kaddr_t)args) < LLD_NUM_TRUNKS
					* sizeof(unsigned int)) {
				seterr(EFAULT);
				return SYSERR;
			}
			if (we->st == NULL) {
				seterr(EINVAL);
				return SYSERR;
			}
			// Cast first to void to avoid compiler warning on ARM
			budgets = (unsigned int *)(void *)args;
			for (i = 0; i < LLD_NUM_TRUNKS; i ++) {
				pIf = &we->if_trunk_array[i];
				budgets[i] = pIf->IfConfig.rx_budget;
			}
			break;
		case LCS_TRIGGER_POLL:
			LldSchedule(NULL, LLD_SCHED_RX);
			break;
		case LCS_ADD_PEER_ENTRY:
			data = (ipandmac_t *)args;
			pIf = lld_ifp_by_name(data->if_name);
			if(pIf == NULL) {
				memcpy(data->err_str, "Interface structure not found\n",
					sizeof(data->err_str));
				seterr(EINVAL);
				return SYSERR;
			}
			ret = update_peer_entry(pIf, data);
			if (ret == SYSERR) {
				return SYSERR;
			}
		break;
		case LCS_DEL_PEER_ENTRY:
			data = (ipandmac_t *)args;
			pIf = lld_ifp_by_name(data->if_name);
			if(pIf == NULL) {
				memcpy(data->err_str, "Interface structure not found\n",
					sizeof(data->err_str));
				seterr(EINVAL);
				return SYSERR;
			}
			ret = delete_peer_entry(pIf, data);
			if (ret == SYSERR) {
				return SYSERR;
			}
		break;
#if (BUILD_MODE == PRODUCTION)
		case LCS_GET_PEER_ENTRY:
			get_peer_entry_t *peer_entry;
			if (wbounds((uaddr_t)args) < sizeof(get_peer_entry_t)) {
				seterr(EFAULT);
				return SYSERR;
			}
			peer_entry = (get_peer_entry_t *)(void *)args;
			if (we->st == NULL) {
				seterr(EINVAL);
				return SYSERR;
			}
			if (peer_entry->if_num >= LLD_NUM_TRUNKS) {
				seterr(EINVAL);
				return SYSERR;
			}
			pIf = &we->if_trunk_array[peer_entry->if_num];
			peer_entry->numOfPeers = pIf->IfConfig.numOfPeers;
			memcpy(peer_entry->if_name, pIf->IfName, sizeof(pIf->IfName));
			if (pIf->IfConfig.ip2macPeers != NULL) {
				memcpy(&(peer_entry->ip2macPeers),
						&(pIf->IfConfig.ip2macPeers),
						PEER_MAX * sizeof(ip2mac_entry_t));
			}
			break;
#endif
		default:
			seterr(EINVAL);
			return SYSERR;
	}
	return OK;
}

#if defined(DLDD)
ATTR_USED static struct dldd entry_points = {
	.dldd_open     = ionull,
	.dldd_close    = ionull,
	.dldd_read     = ioerr,
	.dldd_write    = ioerr,
	.dldd_select   = ioerr,
	.dldd_ioctl    = lcs_ioctl,
	.dldd_install  = lcs_install,
	.dldd_uninstall = lcs_uninstall,
	.dldd_streamtab = NULL
};
#endif // defined(DLDD)

// End of file.
