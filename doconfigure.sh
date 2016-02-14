#!/bin/sh

# Ubuntu
#BASE=/usr/src/linux-headers-

# Generic
#BASE=/usr/src/linux

# Custom
#BASE=/home/tim/src/linux-

# Redhat
BASE=/usr/src/kernels/

# Kernel version (use uname for current version or force if necessary)
#LINUX=$(uname -r)
LINUX=4.3.3-303.fc23.x86_64

# Library directory
#LIB=/lib
#LIB=/usr/lib64

# Debugging?
#DEBUG=--enable-debug
DEBUG=

# Udev (some systems symlink /lib to /usr/lib so it doesn't matter)
#UDEV=/usr/lib/udev

./configure \
	--prefix=/usr \
	--localstatedir=/var \
	--sysconfdir=/etc \
	--sharedstatedir=/com \
	--bindir=/bin \
	--sbindir=/sbin \
	--with-linux=$BASE$LINUX \
	--with-linux-obj=$BASE$LINUX \
	$DEBUG

#	--libdir=$LIB
#	--with-udevdir=$UDEV
