/* <Name>
 * (C) Copyright 2022 MyCompany, Inc.
 * MyTown, MyState   All Rights Reserved.
 *
 * <Description>
 */

#if !defined(__dddrvr_h)
#define __dddrvr_h
/*
 * Basic Template for a Character Device Driver.
 * This template may be used to create your own device driver.
 */

// !!! Locate lines with TODO and modify the file as needed.

#include "ddinfo.h"

// TODO: Add your header files here.

#define DD_DRIVER_NAME __DRVR_NAME__

// TODO: Add or modify your ioctl command definitions.
#define DD1_CMD1 0x4001

#if defined(__Lynx_internal__)

struct dd_device {

	// If select is supported define required variables.
	int *rsel_sem;  // read semaphore
	int *wsel_sem;  // write semaphore
	int *esel_sem;  // exception semaphore
	int in;         // data available for read
	int out;        // write is possible
	int err;        // error condition
	
	char *data;	// data buffer
	int size;	// buffer size
	int datalen;	// received data length
	char *pread;	// read position
	char *pwrite;	// write position

	// TODO:  Add other variables here, e.g.
};

// dd driver statics structure.
struct dd_statics {
	
	// To simulate a character device driver.
	struct dd_device dev[MAXDEVS];
	// TODO:  Add other variables here, e.g.
};
#endif // __LYNXOS

#endif // __dddrvr_h

// End of file.
