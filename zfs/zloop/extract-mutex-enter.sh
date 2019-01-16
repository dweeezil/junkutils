#!/bin/sh
grep 'in mutex_enter ' ztest.gdb | sed -e 's;.*mp=;;' -e 's;.*mp@entry=;;' -e 's;).*;;' > gdb.mutex-enter
