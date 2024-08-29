#!/bin/bash
# vi: set ai ts=4 sw=4 : -*-  tab-width:4  c-basic-offset:4   -*-
# vim: set comments= cinoptions=\:0,t0,+8,c4,C1 :
#****************************************************************************
# (C) Copyright 2021 Lynx Software Technologies, Inc. All rights reserved.

pushd $ENV_PREFIX/sys/drivers.rsc/lcs/config/blobs

[ -d localfs_images ] || mkdir localfs_images

for i in `ls $PWD/img`; do
	name=`basename $i`
	vct="VCT."`basename $i .img`
	spec_in=`basename $i .img`".spec.in"
	echo "spec_in - $spec_in"
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
	sed  's/LCSBLOB/'$name'/g; s/LCSVCT/'$vct'/g' $spec_in > ./localfs.spec
	mkimage -v localfs.spec ./localfs_images/localfs.$name
	rm -rf $i
done
rm -rf localfs.spec
rm -rf $PWD/img
popd
