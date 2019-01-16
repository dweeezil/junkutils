#!/bin/sh
grep -B 1 ztest_execute ztest.gdb | grep -v '^--' | grep -v 'ztest_execute' | grep 'in ztest_' > gdb.execute
