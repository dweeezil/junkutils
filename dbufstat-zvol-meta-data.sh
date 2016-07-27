#!/bin/sh
echo "META:"
(head -3 /proc/spl/kstat/zfs/dbufs ; awk '$8==1 {print $0}' /proc/spl/kstat/zfs/dbufs) | dbufstat.py -i - | grep 'DMU_OT_ZVOL '
echo "DATA:"
(head -3 /proc/spl/kstat/zfs/dbufs ; awk '$8==0 {print $0}' /proc/spl/kstat/zfs/dbufs) | dbufstat.py -i - | grep 'DMU_OT_ZVOL '
