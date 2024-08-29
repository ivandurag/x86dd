/****************************************************
 *                      NOTICE                      *
 * This file is generated automatically by config.  *
 * It should NOT be edited or changed by hand.      *
 ****************************************************/

extern void * lsk_helper_install();		/* LSK Helper driver */
extern ssize_t ioerr_st();		/* null */
extern ssize_t nullwrite();		/* null */
extern int ioerr();		/* null */
extern ssize_t memread();		/* mem */
extern ssize_t memwrite();		/* mem */
extern int cdevopen();		/* ctrl drvr */
extern ssize_t ionull_st();		/* ctrl drvr */
extern ssize_t ionull_st();		/* ctrl drvr */
extern int ttyopen();		/* serial-16550 */
extern int ttyclose();		/* serial-16550 */
extern ssize_t ttyread();		/* serial-16550 */
extern ssize_t ttywrite();		/* serial-16550 */
extern int ttyselect();		/* serial-16550 */
extern int ttyioctl();		/* serial-16550 */
extern void * ttyinstall_16550();		/* serial-16550 */
extern int rrdopen();		/* RAM disk driver (raw) */
extern int rrdclose();		/* RAM disk driver (raw) */
extern ssize_t rrdread();		/* RAM disk driver (raw) */
extern ssize_t rrdwrite();		/* RAM disk driver (raw) */
extern int rrdioctl();		/* RAM disk driver (raw) */
extern void * rrdinstall();		/* RAM disk driver (raw) */
extern int rrduninstall();		/* RAM disk driver (raw) */
extern int rdopen();		/* RAM disk driver (block) */
extern int rdclose();		/* RAM disk driver (block) */
extern ssize_t rdstrategy();		/* RAM disk driver (block) */
extern ssize_t ionull_st();		/* RAM disk driver (block) */
extern int rdioctl();		/* RAM disk driver (block) */
extern void * rdinstall();		/* RAM disk driver (block) */
extern int ptyopen();		/* pty */
extern int ptyclose();		/* pty */
extern ssize_t ptyread();		/* pty */
extern ssize_t ptywrite();		/* pty */
extern int ptyselect();		/* pty */
extern int ptyioctl();		/* pty */
extern void * ptyinstall();		/* pty */
extern int ptyuninstall();		/* pty */
extern ssize_t random_read();		/* random */
extern ssize_t random_write();		/* random */
extern void * random_install();		/* random */
extern int random_uninstall();		/* random */
extern ssize_t ioerr_st();		/* LCS Driver */
extern ssize_t ioerr_st();		/* LCS Driver */
extern int ioerr();		/* LCS Driver */
extern int lcs_ioctl();		/* LCS Driver */
extern void * lcs_install();		/* LCS Driver */
extern int lcs_uninstall();		/* LCS Driver */
extern int lem_open();		/* lcs_em */
extern int lem_close();		/* lcs_em */
extern ssize_t ionull_st();		/* lcs_em */
extern ssize_t ionull_st();		/* lcs_em */
extern int ionull();		/* lcs_em */
extern int lem_ioctl();		/* lcs_em */
extern void * lem_install();		/* lcs_em */
extern int lem_uninstall();		/* lcs_em */
extern int ionull();		/* lcs_igc */
extern int ionull();		/* lcs_igc */
extern ssize_t ionull_st();		/* lcs_igc */
extern ssize_t ionull_st();		/* lcs_igc */
extern int ionull();		/* lcs_igc */
extern int lcs_igc_ioctl();		/* lcs_igc */
extern void * lcs_igc_install();		/* lcs_igc */
extern int ionull();		/* lcs_igc */
extern ssize_t ionull_st();		/* ixgbepf */
extern ssize_t ionull_st();		/* ixgbepf */
extern int ixgbepf_ioctl();		/* ixgbepf */
extern void * ixgbepf_install();		/* ixgbepf */
extern int ionull();		/* LCS VIRTIO */
extern int ionull();		/* LCS VIRTIO */
extern ssize_t ionull_st();		/* LCS VIRTIO */
extern ssize_t ionull_st();		/* LCS VIRTIO */
extern int ionull();		/* LCS VIRTIO */
extern int virtio_net_ioctl();		/* LCS VIRTIO */
extern void * virtio_net_install();		/* LCS VIRTIO */
extern int ionull();		/* LCS VIRTIO */
extern ssize_t ionull_st();		/* ptrace */
extern ssize_t ionull_st();		/* ptrace */
extern void * ptrace_install();		/* ptrace */
extern int unfs_open();		/* unfs */
extern int unfs_close();		/* unfs */
extern ssize_t unfs_read();		/* unfs */
extern ssize_t unfs_write();		/* unfs */
extern int unfs_select();		/* unfs */
extern int unfs_ioctl();		/* unfs */
extern void * unfs_install();		/* unfs */
extern int usbopen();		/* USB */
extern int usbclose();		/* USB */
extern ssize_t usbread();		/* USB */
extern ssize_t ionull_st();		/* USB */
extern int usbioctl();		/* USB */
extern void * usb_install();		/* USB */
extern int usb_uninstall();		/* USB */
extern ssize_t ionull_st();		/* EHCI */
extern ssize_t ionull_st();		/* EHCI */
extern void * ehci_pci_install();		/* EHCI */
extern int ehci_pci_uninstall();		/* EHCI */
extern ssize_t ionull_st();		/* XHCI */
extern ssize_t ionull_st();		/* XHCI */
extern void * xhci_install();		/* XHCI */
extern int ioerr();		/* XHCI */
extern int rsimusb_open();		/* SIM USB */
extern int rsimusb_close();		/* SIM USB */
extern ssize_t rsimusb_read();		/* SIM USB */
extern ssize_t rsimusb_write();		/* SIM USB */
extern int rsimusb_ioctl();		/* SIM USB */
extern void * rsimusb_install();		/* SIM USB */
extern int rsimusb_uninstall();		/* SIM USB */
extern int simusb_open();		/* SIM USB BLK SCSI */
extern int simusb_close();		/* SIM USB BLK SCSI */
extern ssize_t simusb_strategy();		/* SIM USB BLK SCSI */
extern ssize_t ionull_st();		/* SIM USB BLK SCSI */
extern int simusb_ioctl();		/* SIM USB BLK SCSI */
extern void * simusb_install();		/* SIM USB BLK SCSI */
extern int arinc653_open();		/* arinc653 */
extern int arinc653_close();		/* arinc653 */
extern ssize_t ioerr_st();		/* arinc653 */
extern ssize_t ioerr_st();		/* arinc653 */
extern int ioerr();		/* arinc653 */
extern int arinc653_ioctl();		/* arinc653 */
extern void * arinc653_install();		/* arinc653 */
extern int arinc653_uninstall();		/* arinc653 */
extern ssize_t ioerr_st();		/* mmap */
extern ssize_t ioerr_st();		/* mmap */
extern int mmapioctl();		/* mmap */
extern int rahci_open();		/* RAW AHCI */
extern ssize_t rahci_read();		/* RAW AHCI */
extern ssize_t rahci_write();		/* RAW AHCI */
extern int rahci_ioctl();		/* RAW AHCI */
extern void * rahci_install();		/* RAW AHCI */
extern int ahci_open();		/* BLK AHCI */
extern ssize_t ahci_strategy();		/* BLK AHCI */
extern ssize_t ionull_st();		/* BLK AHCI */
extern int ahci_ioctl();		/* BLK AHCI */
extern void * ahci_install();		/* BLK AHCI */
extern ssize_t lsk_fifo_read();		/* LSK FIFO Driver */
extern ssize_t lsk_fifo_write();		/* LSK FIFO Driver */
extern void * lsk_fifo_pci_install();		/* LSK FIFO Driver */
extern int lsk_fifo_uninstall();		/* LSK FIFO Driver */
extern int hm_open();		/* hmdd driver */
extern int hm_close();		/* hmdd driver */
extern ssize_t hm_read();		/* hmdd driver */
extern ssize_t hm_write();		/* hmdd driver */
extern int ioerr();		/* hmdd driver */
extern int hm_ioctl();		/* hmdd driver */
extern void * hm_install();		/* hmdd driver */
extern ssize_t ioerr_st();		/* VMOS driver */
extern ssize_t ioerr_st();		/* VMOS driver */
extern int ioerr();		/* VMOS driver */
extern int vmos_ioctl();		/* VMOS driver */
extern int ioerr();		/* FAT Filesystem */
extern int ioerr();		/* FAT Filesystem */
extern ssize_t ioerr_st();		/* FAT Filesystem */
extern ssize_t ioerr_st();		/* FAT Filesystem */
extern int ioerr();		/* FAT Filesystem */
extern int ioerr();		/* FAT Filesystem */
extern void * lfat_install();		/* FAT Filesystem */
extern int lfat_uninstall();		/* FAT Filesystem */

extern struct dummy gencom_with_skdb;		/* com 1 */
extern struct dummy gencom_without_skdb;		/* com 2 */
extern struct dummy gencom_without_skdb;		/* com 3 */
extern struct dummy gencom_without_skdb;		/* com 4 */
extern struct dummy rdinfo0;		/* RRD */
extern struct dummy rdinfo1;		/* RRD1 */
extern struct dummy ptyinfo;		/* pty 0 */
extern struct dummy ptyinfo;		/* pty 1 */
extern struct dummy ptyinfo;		/* pty 2 */
extern struct dummy ptyinfo;		/* pty 3 */
extern struct dummy ptyinfo;		/* pty 4 */
extern struct dummy ptyinfo;		/* pty 5 */
extern struct dummy ptyinfo;		/* pty 6 */
extern struct dummy ptyinfo;		/* pty 7 */
extern struct dummy lcs_config;		/* LCS Device */
extern struct dummy lcsem_info0;		/* Intel EEPro 1000 LCS Driver */
extern struct dummy ixgbepf_port_info;		/* ixgbe PF 0 */
extern struct dummy unfs_info;		/* unfs */
extern struct dummy usb_info0;		/* USB Device */
extern struct dummy arinc653_info;		/* ARINC653 device */
extern struct dummy ahciinfo0;		/* RAW AHCI */
extern struct dummy lsk_fifoinfo;		/* LSK FIFO Device 0 */
extern struct dummy hminfo0;		/* hmdd */
extern struct dummy rdinfo0;		/* RD */
extern struct dummy rdinfo1;		/* RD1 */


struct udriver_entry udrivers[] = {
	{CHARDRIVER, "LSK Helper driver",
		ionull, ionull, (ssize_t (*)())ionull, (ssize_t (*)())ionull,
		ionull, ionull, lsk_helper_install, ionull, 0},
	{CHARDRIVER, "null",
		ionull, ionull, ioerr_st, nullwrite,
		ionull, ioerr, (void * (*)())ionull, ionull, 0},
	{CHARDRIVER, "mem",
		ionull, ionull, memread, memwrite,
		ionull, ionull, (void * (*)())ionull, ionull, 0},
	{CHARDRIVER, "ctrl drvr",
		cdevopen, ionull, ionull_st, ionull_st,
		ionull, ionull, (void * (*)())ionull, ionull, 0},
	{CHARDRIVER, "serial-16550",
		ttyopen, ttyclose, ttyread, ttywrite,
		ttyselect, ttyioctl, ttyinstall_16550, ionull, 0},
	{CHARDRIVER, "RAM disk driver (raw)",
		rrdopen, rrdclose, rrdread, rrdwrite,
		ionull, rrdioctl, rrdinstall, rrduninstall, 0},
	{BLOCKDRIVER, "RAM disk driver (block)",
		rdopen, rdclose, rdstrategy, ionull_st,
		ionull, rdioctl, rdinstall, ionull, 0},
	{CHARDRIVER, "pty",
		ptyopen, ptyclose, ptyread, ptywrite,
		ptyselect, ptyioctl, ptyinstall, ptyuninstall, 0},
	{CHARDRIVER, "random",
		ionull, ionull, random_read, random_write,
		ionull, ionull, random_install, random_uninstall, 0},
	{CHARDRIVER, "LCS Driver",
		ionull, ionull, ioerr_st, ioerr_st,
		ioerr, lcs_ioctl, lcs_install, lcs_uninstall, 0},
	{CHARDRIVER, "lcs_em",
		lem_open, lem_close, ionull_st, ionull_st,
		ionull, lem_ioctl, lem_install, lem_uninstall, 0},
	{CHARDRIVER, "lcs_igc",
		ionull, ionull, ionull_st, ionull_st,
		ionull, lcs_igc_ioctl, lcs_igc_install, ionull, 0},
	{CHARDRIVER, "ixgbepf",
		ionull, ionull, ionull_st, ionull_st,
		ionull, ixgbepf_ioctl, ixgbepf_install, ionull, 0},
	{CHARDRIVER, "LCS VIRTIO",
		ionull, ionull, ionull_st, ionull_st,
		ionull, virtio_net_ioctl, virtio_net_install, ionull, 0},
	{CHARDRIVER, "ptrace",
		ionull, ionull, ionull_st, ionull_st,
		ionull, ionull, ptrace_install, ionull, 0},
	{CHARDRIVER, "unfs",
		unfs_open, unfs_close, unfs_read, unfs_write,
		unfs_select, unfs_ioctl, unfs_install, ionull, 0},
	{CHARDRIVER, "USB",
		usbopen, usbclose, usbread, ionull_st,
		ionull, usbioctl, usb_install, usb_uninstall, 0},
	{CHARDRIVER, "EHCI",
		ionull, ionull, ionull_st, ionull_st,
		ionull, ionull, ehci_pci_install, ehci_pci_uninstall, 0},
	{CHARDRIVER, "XHCI",
		ionull, ionull, ionull_st, ionull_st,
		ionull, ionull, xhci_install, ioerr, 0},
	{CHARDRIVER, "SIM USB",
		rsimusb_open, rsimusb_close, rsimusb_read, rsimusb_write,
		ionull, rsimusb_ioctl, rsimusb_install, rsimusb_uninstall, 0},
	{BLOCKDRIVER, "SIM USB BLK SCSI",
		simusb_open, simusb_close, simusb_strategy, ionull_st,
		ionull, simusb_ioctl, simusb_install, ionull, 0},
	{CHARDRIVER, "arinc653",
		arinc653_open, arinc653_close, ioerr_st, ioerr_st,
		ioerr, arinc653_ioctl, arinc653_install, arinc653_uninstall, 0},
	{CHARDRIVER, "mmap",
		ionull, ionull, ioerr_st, ioerr_st,
		ionull, mmapioctl, (void * (*)())ionull, ionull, 0},
	{CHARDRIVER, "RAW AHCI",
		rahci_open, ionull, rahci_read, rahci_write,
		ionull, rahci_ioctl, rahci_install, ionull, 0},
	{BLOCKDRIVER, "BLK AHCI",
		ahci_open, ionull, ahci_strategy, ionull_st,
		ionull, ahci_ioctl, ahci_install, ionull, 0},
	{CHARDRIVER, "LSK FIFO Driver",
		ionull, ionull, lsk_fifo_read, lsk_fifo_write,
		ionull, ionull, lsk_fifo_pci_install, lsk_fifo_uninstall, 0},
	{CHARDRIVER, "hmdd driver",
		hm_open, hm_close, hm_read, hm_write,
		ioerr, hm_ioctl, hm_install, ionull, 0},
	{CHARDRIVER, "VMOS driver",
		ionull, ionull, ioerr_st, ioerr_st,
		ioerr, vmos_ioctl, (void * (*)())ionull, ionull, 0},
	{CHARDRIVER, "FAT Filesystem",
		ioerr, ioerr, ioerr_st, ioerr_st,
		ioerr, ioerr, lfat_install, lfat_uninstall, 0}
};
int nudrivers = sizeof(udrivers) / 			sizeof(struct udriver_entry);

struct udevsw_entry ucdevsw[] = {
	{"LSK Helper", 0, 0, -1},
	{"null device", 1, 0, -1},
	{"memory", 2, 0, -1},
	{"ctrl dev", 3, 0, -1},
	{"com 1", 4, (char *)&gencom_with_skdb, -1},
	{"com 2", 4, (char *)&gencom_without_skdb, -1},
	{"com 3", 4, (char *)&gencom_without_skdb, -1},
	{"com 4", 4, (char *)&gencom_without_skdb, -1},
	{"RRD", 5, (char *)&rdinfo0, -1},
	{"RRD1", 5, (char *)&rdinfo1, -1},
	{"pty 0", 7, (char *)&ptyinfo, -1},
	{"pty 1", 7, (char *)&ptyinfo, -1},
	{"pty 2", 7, (char *)&ptyinfo, -1},
	{"pty 3", 7, (char *)&ptyinfo, -1},
	{"pty 4", 7, (char *)&ptyinfo, -1},
	{"pty 5", 7, (char *)&ptyinfo, -1},
	{"pty 6", 7, (char *)&ptyinfo, -1},
	{"pty 7", 7, (char *)&ptyinfo, -1},
	{"random device", 8, 0, -1},
	{"LCS Device", 9, (char *)&lcs_config, -1},
	{"Intel EEPro 1000 LCS Driver", 10, (char *)&lcsem_info0, -1},
	{"Intel I225GM LCS Driver", 11, 0, -1},
	{"ixgbe PF 0", 12, (char *)&ixgbepf_port_info, -1},
	{"VIRTIO NIC 0", 13, 0, -1},
	{"VIRTIO NIC 1", 13, 0, -1},
	{"VIRTIO NIC 2", 13, 0, -1},
	{"VIRTIO NIC 3", 13, 0, -1},
	{"VIRTIO NIC 4", 13, 0, -1},
	{"VIRTIO NIC 5", 13, 0, -1},
	{"VIRTIO NIC 6", 13, 0, -1},
	{"VIRTIO NIC 7", 13, 0, -1},
	{"ptrace", 14, 0, -1},
	{"unfs", 15, (char *)&unfs_info, -1},
	{"USB Device", 16, (char *)&usb_info0, -1},
	{"EHCI controller", 17, 0, -1},
	{"XHCI controller", 18, 0, -1},
	{"SIM USB RAW SCSI", 19, 0, -1},
	{"ARINC653 device", 21, (char *)&arinc653_info, -1},
	{"mmap", 22, 0, -1},
	{"RAW AHCI", 23, (char *)&ahciinfo0, -1},
	{"LSK FIFO Device 0", 25, (char *)&lsk_fifoinfo, -1},
	{"hmdd", 26, (char *)&hminfo0, -1},
	{"VMOS device", 27, 0, -1},
	{"FAT Filesystem", 28, 0, -1}
};
int nucdevsw = sizeof(ucdevsw) / 			sizeof(struct udevsw_entry);

struct udevsw_entry ubdevsw[] = {
	{"RD", 6, (char *)&rdinfo0, 8},
	{"RD1", 6, (char *)&rdinfo1, 9},
	{"SIM USB SCSI", 20, 0, 36},
	{"BLK AHCI", 24, 0, 39}
};
int nubdevsw = sizeof(ubdevsw) / 			sizeof(struct udevsw_entry);

struct fmodsw fmod_sw[] = {
	{"<noname>", 0}
};
int nfmodsw = 0;
