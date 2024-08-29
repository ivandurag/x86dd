/* <Name>
 * (C) Copyright 2021 MyCompany, Inc.
 * MyTown, MyState   All Rights Reserved.
 *
 * <Description>
 */

/* 
 * Basic Template for a Character Device Driver.
 *
 * This template may be used to create your own device driver.
 *
 *  This driver is a standard template for a character driver. It
 *  provides all the entry points in a standard LynxOS character device
 *  driver and is platform independent. This driver can be compiled and
 *  installed into any LynxOS system which supports dynamic
 *  installation of device drivers. It can also be used as a basic
 *  template for any driver development on LynxOS.
 */

/* !!! Locate lines with TODO and modify the file as needed */

#include <kernel.h>             // SYSERR
#include <errno.h>              // ENOMEM
#include <string.h>             // memset
#include <sys/ioctl.h>          // FIOPRIO, FIOASYNC
#if defined(DLDD)
#include <dldd.h>               // dldd
#endif

#include "ddinfo.h"            // dd_info, MAXDEVS
#include "dddrvr.h"

// TODO: Add your header files here.

// Debugging support.
#if defined(_DEBUG)
#define TRACE(s) {cprintf(s);}
#define TRACE1(s, p) {cprintf(s, p);}
#define TRACE2(s, p1, p2) {cprintf(s, p1, p2);}
#define TRACE2_TEXT(s, p1, p2) trace_buf_safe((s), (long)(p1), (p2))
#define TRACE_ENTER(s) TRACE1("dd: %s entered\n", s)
#define TRACE_EXIT(s) TRACE1("dd: %s exits\n", s)
#else
#define TRACE(s) 
#define TRACE1(s, p) 
#define TRACE2(s, p1, p2)
#define TRACE2_TEXT(s, p1, p2)
#define TRACE_ENTER(s) 
#define TRACE_EXIT(s) 
#endif


#if defined (_DEBUG)
char *estr = "";

static char *
add_nl_str(char *buf, long size)
{
	char *str = 0;

	str = (char *)sysbrk(size + 1);
	if (str == 0) {
		return estr;
	}
	memcpy(str, buf, size);
	str[size] = '\0';
	return str;
}

static void
trace_buf_safe(const char *fmt, long size, char *buf)
{
	char *str = add_nl_str(buf, size);

	TRACE2((char *)fmt, size, str);

	sysfree((char *)str, size);
}
#endif

/* 
* Name: dd_install
*
* Description: 
* 
* The install entry point is the entry point which is invoked by LynxOS when 
* the device driver is first installed in the system. (through the devinstall 
* utility)
*
* Input: dd_info structure pointer
*
* Returns: pointer to the statics structure
*/ 

void *
dd_install(struct dd_info *info)
{
	struct	dd_statics *s;
	int i;

	TRACE1("Device is installed with info->size value %d\n", info->size);
	/*
	 * TODO: Insert the code here to check if the device is present.
	 * If the device is absent, return ((char *)SYSERR).
	 */

	// Allocate statics structure.
	s = (struct dd_statics *)sysbrk(sizeof(struct dd_statics));
	if (s == 0) {
		TRACE("ddinstall: Cannot get memory for statics structure.\n");
		pseterr(ENOMEM);
		return (void *)SYSERR;
	}
	// Clean statics.
	memset(s, 0, sizeof(struct dd_statics));

	// TODO: Set other fields in statics structure, e.g.

	// TODO: Initialize the device, attach interrupt handler, etc.

	// To simulate a device driver, allocate a device buffer.
	for (i = 0; i < MAXDEVS; i++) {
		struct dd_device *d = &s->dev[i];

		// If select is supported initialize the select variables.
		d->in = d->out = d->err = 0;
		
		d->size = info->size;

		d->data = (char *)sysbrk(d->size);
		if (d->data == 0) {
			TRACE1("dd_install: Cannot get memory. Size = %d\n",
					d->size);
			pseterr(ENOMEM);
			return (void *)SYSERR;
		}
		// Clean data buffer.
		memset(d->data, 0, d->size);
		d->datalen = 0;
		d->pread = d->pwrite = d->data;
	}

	TRACE_EXIT("install");

	// Return allocated statics structure.
	return s;
}

/* 
* Name: dd_uninstall
* 
* Description: 
* The uninstall routine is called when the driver is uninstalled.
* 
* Input: dd_statics structure pointer
* 
* Returns: OK or SYSERR
*/
int
dd_uninstall(struct dd_statics *s)
{
	int i;

	TRACE_ENTER("uninstall");
	/*
	 * TODO: Add device specific code to clear interrupt vectors,
	 * disable the device, etc.
	 */
	for (i = 0; i < MAXDEVS; i++) {
		struct dd_device *d = &s->dev[i];

		sysfree((char *)d->data, d->size);
	}
	sysfree((char *)s, sizeof(struct dd_statics));
	
	TRACE_EXIT("uninstall");

	return OK;
}

/* 
* Name: dd_open
* 
* Description: The open routine is called upon an application open of the
* device.
* 
* Input: statics pointer, device number, file structure pointer.
* 
* Returns: OK or SYSERR
*/
int
dd_open(struct dd_statics *s, int dev, struct file *f)
{
	TRACE2("dd: open called for device major=%d, minor=%d\n\n", 
			major(dev), minor(dev));
	
	/*
	 * TODO: Add minor device initializing code here.
	 * Use macro major(dev) and minor(dev) to get
	 * the major and minor device numbers.
	 */
	
	TRACE_EXIT("open");

	return OK;
}

/* 
* Name: dd_close
* 
* Description: The close routine is called upon an application close of the
* device.
* 
* Input: dd_statics pointer, file structure pointer
* 
* Returns: OK or SYSERR
*/
int
dd_close(struct dd_statics *s, struct file *f)
{
	TRACE_ENTER("close");

	// TODO: Deallocate any resources if necessary.
	
	TRACE_EXIT("close");

	return OK;
}

/* 
* Name: dd_read
* 
* Description: The read routine is called upon an application read from the
* device.
* 
* Input: dd_statics pointer, file structure pointer, buffer pointer,
* count of bytes to read.
* 
* Returns: count of the bytes actually read.
*/
ssize_t
dd_read(struct dd_statics *s, struct file *f, char *buff,
        size_t count)
{
	int devn = minor(f->dev);
	struct dd_device *dev = &s->dev[devn];
	char *bufend = dev->data + dev->size -1;
	int datalen = dev->datalen;
	int bytes_to_read = count;
	int rbytes = 0;

	TRACE1("dd_read entered to read %d bytes\n", count);

	if (datalen < 1) {
		// No data to read.
		return 0;
	}

	// TODO: Add code to move read data into the buffer.

	// This is a simulation of the character driver.
	if (bytes_to_read > datalen) {
		bytes_to_read = datalen;
	}
	while (bytes_to_read) {

			*buff++ = *dev->pread++;
			if (dev->pread > bufend) {
				dev->pread = dev->data;
			}
			bytes_to_read--;
			rbytes++;
	}
	dev->datalen -= rbytes;
	
	TRACE1("dd_read returned %d bytes\n", rbytes);
	return rbytes;
}

/* 
* Name: dd_write
* 
* Description: The write routine is called upon an application write to the
* device.
* 
* Input: dd_statics pointer, file structure pointer, buffer pointer,
* count of bytes to write.
* 
* Returns: count of the bytes actually written.
*/
ssize_t
dd_write(struct dd_statics *s, struct file *f, char *buff, size_t count)
{
	int devn = minor(f->dev);
	struct dd_device *dev = &s->dev[devn];
	char *bufend = dev->data + dev->size - 1;
	int freespace = dev->size - dev->datalen;
	int bytes_to_write = count;
	int wbytes = 0;

	TRACE2_TEXT("dd: write called to copy %d bytes from buffer \"%s\"\n",
			count, buff);

	if (freespace < 1) {
		// No free space in cycle buffer.
		pseterr(EAGAIN);
		return SYSERR;
	}

	// TODO: Add code to write data from the buffer to the device.

	// This is a simulation of the character driver.
	if (bytes_to_write > freespace) {
		bytes_to_write = freespace;
	}
	while (bytes_to_write) {

			*dev->pwrite++ = *buff++;
			if (dev->pwrite > bufend) {
				dev->pwrite = dev->data;
			}
			bytes_to_write--;
			wbytes++;
	}
	dev->datalen += wbytes;

	TRACE_EXIT("write");
	return wbytes;
}

/* 
* Name: dd_ioctl
* 
* Description: The ioctl is called upon the application ioctl to the device
* 
* Input: dd_statics pointer, file structure pointer, command,
* argument to command.
* 
* Returns: OK or SYSERR
*/
int
dd_ioctl(struct dd_statics *s, struct file *f, int cmd, char *args)
{
	TRACE1("dd: ioctl entry point is called for command %x\n", cmd);

	switch (cmd) {
		// TODO: Depending on the command, add appropriate code.
		case FIOPRIO:
			break;

		case FIOASYNC:
			break;

		case DD1_CMD1:
			break;

		default:
			pseterr(EINVAL);
			return SYSERR;
	}

	TRACE_EXIT("ioctl");

	return OK;
}

/* 
* Name: dd_select
* 
* Description: The select is called upon the application invocation of the
* select system call.
* 
* Input: dd_statics pointer, file structure pointer, which command,
* select structure pointer.
* 
* Returns: OK or SYSERR
*/
int
dd_select(struct dd_statics *s, struct file *f, int which,
        struct sel *ffs)
{
	int devn = minor(f->dev);
	struct dd_device *dev = &s->dev[devn];

	switch (which) {

	case SREAD:
		ffs->iosem = &dev->in;
		ffs->sel_sem = &dev->rsel_sem;
		break;

	case SWRITE:
		ffs->iosem = &dev->out;
		ffs->sel_sem = &dev->wsel_sem;
		break;

	case SEXCEPT:
		ffs->iosem = &dev->err;
		ffs->sel_sem = &dev->esel_sem;
		break;

	default:
		pseterr(EINVAL);
		return SYSERR;
	}
	return OK;
}

#if defined(DLDD)
struct dldd entry_points = {
	dd_open,
	dd_close,
	dd_read,
	dd_write,
	dd_select,
	dd_ioctl,
	dd_install,
	dd_uninstall,
	(kaddr_t)0
};
#endif

// End of file.
