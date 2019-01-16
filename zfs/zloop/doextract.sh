#!/bin/sh

X=`pwd`
for i in /var/tmp/zloop/*; do
	cd $i
	~/extract-mutex-call.sh
	~/extract-mutex-enter.sh
	~/extract-ztest-exec.sh
done
cd "$X"
