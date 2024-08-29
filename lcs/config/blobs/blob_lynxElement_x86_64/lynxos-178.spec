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






directory=/etc
 file=starttab   source=$(ENV_PREFIX)/etc/starttab      owner=0 group=0 mode=-rw-r--r--
 file=termcap    source=$(ENV_PREFIX)/etc/termcap       owner=0 group=0 mode=-rw-r--r--

 file=ttys       source=$(ENV_PREFIX)/etc/ttys          owner=0 group=0 mode=-rw-r--r--
 file=tconfig    source=$(ENV_PREFIX)/etc/tconfig       owner=0 group=0 mode=-rw-r--r--
 file=shells     source=$(ENV_PREFIX)/etc/shells        owner=0 group=0 mode=-rw-r--r--
 file=passwd     source=$(ENV_PREFIX)/etc/passwd        owner=0 group=0 mode=-rw-r--r--
#file=resolv.conf source=$(ENV_PREFIX)/etc/resolv.conf  owner=0 group=0 mode=-rw-r--r--

directory=/usr/local

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
