# vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*-
# vim: set comments= cinoptions=\:0,t0,+8,c4,C1 :
#****************************************************************************
# (C) Copyright 2023 Lynx Software Technologies, Inc. All rights reserved.
#****************************************************************************



kernel=$(ENV_PREFIX)/sys/bsp.x86_64/a.out
nodetab=$(ENV_PREFIX)/sys/bsp.x86_64/nodetab

target=$(LYNXTARGET)
osstrip=false
ostext=ram
elf=false

# File System
free=100
inodes=10
root=ram

# Text Files
strip=false
text=ram
resident=false


directory=/                                     owner=0 group=0 mode=dr-xr-xr-x
# file=init      source=$(ENV_PREFIX)/bin/hello  owner=0 group=0 mode=-r-xr-xr-x
file=init      source=$(ENV_PREFIX)/bin/cinit  owner=0 group=0 mode=-r-xr-xr-x
 file=.profile  source=$(ENV_PREFIX)/etc/profile.sample owner=0 group=0 mode=-r-xr-xr-x

directory=/bin                                  owner=0 group=0 mode=dr-xr-xr-x
 file=hello     source=$(ENV_PREFIX)/bin/hello  owner=0 group=0 mode=-r-xr-xr-x
 file=cinit     source=$(ENV_PREFIX)/bin/cinit  owner=0 group=0 mode=-r-xr-xr-x
 file=mkffs     source=$(ENV_PREFIX)/bin/mkffs  owner=0 group=0 mode=-r-xr-xr-x
 file=ffsck     source=$(ENV_PREFIX)/bin/ffsck  owner=0 group=0 mode=-r-xr-xr-x


# Common tools
 file=awk       source=$(ENV_PREFIX)/bin/awk    owner=0 group=0 mode=-r-xr-xr-x
 file=bash      source=$(ENV_PREFIX)/bin/bash   owner=0 group=0 mode=-r-xr-xr-x
 link /bin/bash /bin/sh
 file=cat       source=$(ENV_PREFIX)/bin/cat    owner=0 group=0 mode=-r-xr-xr-x
 file=chmod     source=$(ENV_PREFIX)/bin/chmod  owner=0 group=0 mode=-r-xr-xr-x
 file=chown     source=$(ENV_PREFIX)/bin/chown  owner=0 group=0 mode=-r-xr-xr-x
 file=cp        source=$(ENV_PREFIX)/bin/cp     owner=0 group=0 mode=-r-xr-xr-x
 file=date      source=$(ENV_PREFIX)/bin/date   owner=0 group=0 mode=-r-xr-xr-x
 file=file      source=$(ENV_PREFIX)/bin/file   owner=0 group=0 mode=-r-xr-xr-x
 file=find      source=$(ENV_PREFIX)/bin/find   owner=0 group=0 mode=-r-xr-xr-x
 file=gdbserver source=$(ENV_PREFIX)/bin/gdbserver owner=0 group=0 mode=-r-xr-xr-x
 file=grep      source=$(ENV_PREFIX)/bin/grep   owner=0 group=0 mode=-r-xr-xr-x
 file=lipcrm    source=$(ENV_PREFIX)/bin/lipcrm owner=0 group=0 mode=-r-xr-xr-x
 file=lipcs     source=$(ENV_PREFIX)/bin/lipcs  owner=0 group=0 mode=-r-xr-xr-x
 file=ln        source=$(ENV_PREFIX)/bin/ln     owner=0 group=0 mode=-r-xr-xr-x
 file=login     source=$(ENV_PREFIX)/bin/login  owner=0 group=0 mode=-r-xr-xr-x
 file=ls        source=$(ENV_PREFIX)/bin/ls     owner=0 group=0 mode=-r-xr-xr-x
 file=mv        source=$(ENV_PREFIX)/bin/mv     owner=0 group=0 mode=-r-xr-xr-x
 file=prio      source=$(ENV_PREFIX)/bin/prio   owner=0 group=0 mode=-r-xr-xr-x
 file=ps        source=$(ENV_PREFIX)/bin/ps     owner=0 group=0 mode=-r-xr-xr-x
 file=rc        source=$(ENV_PREFIX)/bin/rc     owner=0 group=0 mode=-r-xr-xr-x
 file=rdc       source=$(ENV_PREFIX)/bin/rdc    owner=0 group=0 mode=-r-xr-xr-x
 file=reboot    source=$(ENV_PREFIX)/bin/reboot owner=0 group=0 mode=-r-xr-xr-x
 file=rm        source=$(ENV_PREFIX)/bin/rm     owner=0 group=0 mode=-r-xr-xr-x
 file=runshell source=$(ENV_PREFIX)/bin/runshell owner=0 group=0 mode=-r-xr-xr-x
 file=sleep     source=$(ENV_PREFIX)/bin/sleep  owner=0 group=0 mode=-r-xr-xr-x
#file=tail      source=$(ENV_PREFIX)/bin/tail   owner=0 group=0 mode=-r-xr-xr-x
 file=tset      source=$(ENV_PREFIX)/bin/tset   owner=0 group=0 mode=-r-xr-xr-x
 file=uname     source=$(ENV_PREFIX)/bin/uname  owner=0 group=0 mode=-r-xr-xr-x

# Disk utilities
 file=df        source=$(ENV_PREFIX)/bin/df     owner=0 group=0 mode=-r-xr-xr-x
 file=mkdir     source=$(ENV_PREFIX)/bin/mkdir  owner=0 group=0 mode=-r-xr-xr-x
 file=mkfs      source=$(ENV_PREFIX)/bin/mkfs   owner=0 group=0 mode=-r-xr-xr-x
 file=mknod     source=$(ENV_PREFIX)/bin/mknod  owner=0 group=0 mode=-r-xr-xr-x
 file=mkpart    source=$(ENV_PREFIX)/bin/mkpart owner=0 group=0 mode=-r-xr-xr-x
 file=mount     source=$(ENV_PREFIX)/bin/mount  owner=0 group=0 mode=-r-sr-xr-x
 file=sync      source=$(ENV_PREFIX)/bin/sync   owner=0 group=0 mode=-r-xr-xr-x
 file=umount    source=$(ENV_PREFIX)/bin/umount owner=0 group=0 mode=-r-sr-xr-x

# NW utilities
 file=ftp      source=$(ENV_PREFIX)/bin/ftp     owner=0 group=0 mode=-r-xr-xr-x
 file=ifconfig source=$(ENV_PREFIX)/bin/ifconfig owner=0 group=0 mode=-r-xr-xr-x
 file=lcsnetstat source=$(ENV_PREFIX)/bin/lcsnetstat owner=0 group=0 mode=-r-xr-xr-x
 file=ping6     source=$(ENV_PREFIX)/bin/ping6  owner=0 group=0 mode=-r-xr-xr-x
 file=ping      source=$(ENV_PREFIX)/bin/ping   owner=0 group=0 mode=-r-xr-xr-x
 file=rcp       source=$(ENV_PREFIX)/bin/rcp    owner=0 group=0 mode=-r-xr-xr-x
 file=rlogin    source=$(ENV_PREFIX)/bin/rlogin owner=0 group=0 mode=-r-xr-xr-x
 file=route     source=$(ENV_PREFIX)/bin/route  owner=0 group=0 mode=-r-xr-xr-x
 file=rsh       source=$(ENV_PREFIX)/bin/rsh    owner=0 group=0 mode=-r-xr-xr-x
 file=telnet    source=$(ENV_PREFIX)/bin/telnet owner=0 group=0 mode=-r-xr-xr-x

# NW NTP
 file=ntpdate   source=$(ENV_PREFIX)/bin/ntpdate owner=0 group=0 mode=-r-xr-xr-x
 file=ntpq      source=$(ENV_PREFIX)/bin/ntpq    owner=0 group=0 mode=-r-xr-xr-x
 file=ntptrace  source=$(ENV_PREFIX)/bin/ntptrace owner=0 group=0 mode=-r-xr-xr-x
 file=xntpd     source=$(ENV_PREFIX)/bin/xntpd  owner=0 group=0 mode=-r-xr-xr-x
 file=xntpdc    source=$(ENV_PREFIX)/bin/xntpdc owner=0 group=0 mode=-r-xr-xr-x

# Devices and drivers tools
 file=devices   source=$(ENV_PREFIX)/bin/devices owner=0 group=0 mode=-r-xr-xr-x
 file=devinstall source=$(ENV_PREFIX)/bin/devinstall owner=0 group=0 mode=-r-xr-xr-x
 file=drinstall source=$(ENV_PREFIX)/bin/drinstall owner=0 group=0 mode=-r-xr-xr-x
 file=drivers   source=$(ENV_PREFIX)/bin/drivers owner=0 group=0 mode=-r-xr-xr-x
 file=drm_stat  source=$(ENV_PREFIX)/bin/drm_stat owner=0 group=0 mode=-r-xr-xr-x
 file=usbdevs   source=$(ENV_PREFIX)/bin/usbdevs owner=0 group=0 mode=-r-xr-xr-x
 file=fsck.fat  source=$(ENV_PREFIX)/bin/fsck.fat  owner=0 group=0 mode=-r-xr-xr-x
 file=mkfs.fat  source=$(ENV_PREFIX)/bin/mkfs.fat  owner=0 group=0 mode=-r-xr-xr-x



directory=/home/ftp                             owner=600 group=2 mode=drwxr-xr-x
directory=/home/vl                              owner=601 group=2 mode=drwxr-xr-x

directory=/usr                                  owner=0 group=0 mode=drwxr-xr-x
  symlink /bin    /usr/bin

directory=/usr/etc                              owner=0 group=0 mode=drwxr-xr-x
 file=VCT source=./VCT                          owner=0 group=0 mode=-rw-r--r--

directory=/usr/local                            owner=0 group=0 mode=drwxr-xr-x

directory=/usr/local/etc                        owner=0 group=0 mode=drwxr-xr-x
 file=VCT source=./VCT                          owner=0 group=0 mode=-rw-r--r--

directory=/usr/local/etc/dropbear owner=0 group=0 mode=drwx------
 file=dropbear_ecdsa_host_key source=$(ENV_PREFIX)/etc/dropbear/dropbear_ecdsa_host_key

#directory=usr/ptrace                            owner=0 group=0 mode=drwxr-xr-x
# file=ptrace.dldd source=$(ENV_PREFIX)/sys/dldd/ptrace_x86.dldd owner=0 group=0 mode=-rw-r--r--
# file=ptrace.info source=$(ENV_PREFIX)/sys/dldd/ptrace_x86.info owner=0 group=0 mode=-rw-r--r--

directory=/etc
 file=starttab   source=$(ENV_PREFIX)/etc/starttab      owner=0 group=0 mode=-rw-r--r--
 file=termcap    source=$(ENV_PREFIX)/etc/termcap       owner=0 group=0 mode=-rw-r--r--

 symlink /tmp/mtab /etc/mtab
 file=magic      source=$(ENV_PREFIX)/etc/magic         owner=0 group=0 mode=-rw-r--r--
 file=hosts      source=$(ENV_PREFIX)/etc/hosts         owner=0 group=0 mode=-rw-r--r--
 file=protocols  source=$(ENV_PREFIX)/etc/protocols     owner=0 group=0 mode=-rw-r--r--
 file=services   source=$(ENV_PREFIX)/etc/services      owner=0 group=0 mode=-rw-r--r--
 file=inetd.conf source=$(ENV_PREFIX)/etc/inetd.conf    owner=0 group=0 mode=-rw-r--r--

 file=ttys       source=$(ENV_PREFIX)/etc/ttys          owner=0 group=0 mode=-rw-r--r--
 file=tconfig    source=$(ENV_PREFIX)/etc/tconfig       owner=0 group=0 mode=-rw-r--r--
 file=shells     source=$(ENV_PREFIX)/etc/shells        owner=0 group=0 mode=-rw-r--r--
 file=passwd     source=$(ENV_PREFIX)/etc/passwd        owner=0 group=0 mode=-rw-r--r--
#file=resolv.conf source=$(ENV_PREFIX)/etc/resolv.conf  owner=0 group=0 mode=-rw-r--r--

directory=/net

 file=unfsio     source=$(ENV_PREFIX)/net/unfsio        owner=0 group=0 mode=-r-xr-xr-x
 file=rshd       source=$(ENV_PREFIX)/net/rshd          owner=0 group=0 mode=-r-xr-xr-x
 file=rlogind    source=$(ENV_PREFIX)/net/rlogind       owner=0 group=0 mode=-r-xr-xr-x
 file=telnetd    source=$(ENV_PREFIX)/net/telnetd       owner=0 group=0 mode=-r-xr-xr-x
 file=ftpd       source=$(ENV_PREFIX)/net/ftpd          owner=0 group=0 mode=-r-xr-xr-x
 file=inetd      source=$(ENV_PREFIX)/net/inetd         owner=0 group=0 mode=-r-xr-xr-x
 file=rc.network source=$(ENV_PREFIX)/net/rc.network    owner=0 group=0 mode=-r-xr-xr-x

 file=dropbearmulti source=$(ENV_PREFIX)/net/dropbearmulti owner=0 group=0 mode=-r-xr-xr-x
 link /net/dropbearmulti /bin/dbclient
 link /net/dropbearmulti /net/dropbear
 link /net/dropbearmulti /net/dropbearkey
 link /net/dropbearmulti /bin/scp
 link /net/dropbearmulti /bin/ssh


# The home for device nodes
directory=/dev                                  owner=0 group=0 mode=drwxr-xr-x

# Mount point for ram disk for dynamic device driver nodes
directory=/dev/ddev                             owner=0 group=0 mode=drwxr-xr-x

# Mount point for RAM disk
directory=/tmp                                  owner=0 group=0 mode=drwxrwxrwx

# The filesystems listed in the VCT are mounted under /mnt
directory=/mnt                                  owner=0 group=0 mode=drwxrwxrwx
directory=/mnt/a                                owner=0 group=0 mode=drwxrwxrwx
directory=/mnt/b                                owner=0 group=0 mode=drwxrwxrwx
directory=/mnt/c                                owner=0 group=0 mode=drwxrwxrwx
directory=/mnt/d                                owner=0 group=0 mode=drwxrwxrwx

# Symlink for default location for CoRE files
symlink /tmp /rwfs

# End of file.
