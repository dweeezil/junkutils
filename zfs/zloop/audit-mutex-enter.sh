#!/bin/sh
X=`pwd`
cd /var/tmp/zloop
cat */gdb.mutex-enter | sort | uniq -c | sort -n -k 1,1
cd "$X"
