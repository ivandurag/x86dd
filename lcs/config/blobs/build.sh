#!/bin/bash
# vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*-
# vim: set comments= cinoptions=\:0,t0,+8,c4,C1 :
#****************************************************************************
# (C) Copyright 2021-2023 Lynx Software Technologies, Inc. All rights reserved.

# Check if argument is blank or help
if [ "$1" == "?" ] || [ "$1" == "-h" ] || [ "$1" == "" ]; then
echo "usage ./build.sh <blob-directory-name>"
exit
fi

if [ ! -d $1 ] ; then
echo "Directory doesn't exist: $1"
exit
fi

basedir=${1%/}
pushd $1

echo "generating LCS configuration blobs"

blob_src=`find . -name "*.c"`

[ -d $PWD/img ] || mkdir $PWD/img

for f in $blob_src; do
	fn=`basename $f .c`
# TODO: this should be build through Makefile, so that it would pick up appropriate CFLAGS
	gcc -I $ENV_PREFIX/usr/include -I $ENV_PREFIX/sys/include -I $ENV_PREFIX/sys/include/family/$LYNXTARGET \
			-D__Lynx_kernel__ -D__Lynx_internal__ -c $f -o $fn.o
	objcopy -R .text -R .note -R .comments $fn.o $fn.stripped.o
	objcopy -O binary $fn.stripped.o img/$fn.img
done
rm -rf *.o


echo "Building localfs images"

[ -d localfs_images ] || mkdir localfs_images

for i in `ls $PWD/img`; do
	echo "Current directory: $basedir"
	name=`basename $i`
	vct="VCT."`basename $i .img`
	spec_in=`basename $i .img`".spec.in"
	echo "spec_in: $spec_in"
	echo "Image: $name"

	if [ ! -f "$PWD/$vct" ]; then
		echo -e "\nWARNING: Custom $vct is not found,"
		echo -en "Do you want to use BSP default? (yes/[no])? "
		read answer
		case "$answer" in
			[yY][eE][sS]|[yY])
			# If VCT does not exist copy the platform default
			if [ ! -f $ENV_PREFIX/sys/bsp.$BOARD/VCT ]; then
				echo "Please build BSP first."
				exit
			fi
			cp $ENV_PREFIX/sys/bsp.$BOARD/VCT $vct ;;
			*) # Otherwise
			echo -en "\n\tPLEASE PROVIDE CUSTOM VCT for\n\t$name LCS INFO\n\n"
			exit ;;
		esac
	fi
	sed  's/LCSBLOB/'$name'/g; s/LCSVCT/'$vct'/g; s/BLOBS/'$basedir'/g' $spec_in > ./localfs.spec
	mkimage -v localfs.spec ./localfs_images/localfs.$name
	rm -rf $i
done
rm -rf localfs.spec
rm -rf $PWD/img
popd
