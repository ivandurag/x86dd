#!/bin/sh

# Configure the lcs0 IPv4 and IPv6
ifconfig lcs0 10.168.20.21 up
ifconfig lcs0 inet6 5001:db8:4672:6565:2026:5044:2d44:50d1/64 up

ifconfig -a

lcsnetstat -n

/net/inetd
