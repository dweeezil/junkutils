#!/bin/sh
grep -A 1 'in mutex_enter ' ztest.gdb | grep -v -- '^--' | grep -v ' in mutex_enter ' | grep -v '__left = ' > gdb.mutex-call
