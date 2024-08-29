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





directory=/home/ftp                             owner=600 group=2 mode=drwxr-xr-x
directory=/home/vl                              owner=601 group=2 mode=drwxr-xr-x

directory=/usr                                  owner=0 group=0 mode=drwxr-xr-x
  symlink /bin    /usr/bin

directory=/usr/etc                              owner=0 group=0 mode=drwxr-xr-x
 file=VCT source=./VCT                          owner=0 group=0 mode=-rw-r--r--

directory=/usr/local                            owner=0 group=0 mode=drwxr-xr-x

directory=/usr/local/etc                        owner=0 group=0 mode=drwxr-xr-x
# file=VCT source=./VCT                          owner=0 group=0 mode=-rw-r--r--

directory=/usr/local/etc/dropbear owner=0 group=0 mode=drwx------
 file=dropbear_ecdsa_host_key source=$(ENV_PREFIX)/etc/dropbear/dropbear_ecdsa_host_key

#directory=usr/ptrace                            owner=0 group=0 mode=drwxr-xr-x
# file=ptrace.dldd source=$(ENV_PREFIX)/sys/dldd/ptrace_x86.dldd owner=0 group=0 mode=-rw-r--r--
# file=ptrace.info source=$(ENV_PREFIX)/sys/dldd/ptrace_x86.info owner=0 group=0 mode=-rw-r--r--

directory=/etc
 file=starttab   source=$(ENV_PREFIX)/etc/starttab      owner=0 group=0 mode=-rw-r--r--
 file=termcap    source=$(ENV_PREFIX)/etc/termcap       owner=0 group=0 mode=-rw-r--r--

 file=ttys       source=$(ENV_PREFIX)/etc/ttys          owner=0 group=0 mode=-rw-r--r--
 file=tconfig    source=$(ENV_PREFIX)/etc/tconfig       owner=0 group=0 mode=-rw-r--r--
 file=shells     source=$(ENV_PREFIX)/etc/shells        owner=0 group=0 mode=-rw-r--r--
 file=passwd     source=$(ENV_PREFIX)/etc/passwd        owner=0 group=0 mode=-rw-r--r--
#file=resolv.conf source=$(ENV_PREFIX)/etc/resolv.conf  owner=0 group=0 mode=-rw-r--r--

directory=/net


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
