#!/bin/sh
X=`pwd`
cd /var/tmp/zloop
grep ' at \.\.' */gdb.mutex-call | sed -e 's;.* at \.\.;;' | sort | uniq -c | sort -n -k 1,1
cd "$X"
