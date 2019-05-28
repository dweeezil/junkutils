#!/bin/bash

# getstacks.sh - Process the output of "dmesg", typically following a
# "echo w > /proc/sysrq-trigger" (show blocked tasks) and produce a list
# of all the unique stack traces.

sed -e 's;^\[[0-9. ][0-9. ]*\]  *;;' -e 's;+0x.*;;' |
	grep -v ' D ' |
	grep -v '^Code:' |
	grep -v '^Workqueue:' |
	grep -v '^sysrq:' |
	grep -v '^task ' |
	grep -v '^? ' |
	grep -v '^R[A-Z0-9][A-Z0-9]*:' |
	while read line; do
		if [ "$line" = "Call Trace:" ]; then
			echo ""
		else
			echo -n " $line"
		fi
	done |
	sed -e 's;^ ;;' |
	sort |
	uniq -c
