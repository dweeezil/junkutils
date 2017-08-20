#!/bin/sh

AS=/proc/spl/kstat/zfs/arcstats

# arc_meta_used =
#   metadata_size +
#   bonus_size +
#   dnode_size +
#   dbuf_size +
#   hdr_size +
#   l2_hdr_size
#
egrep '^(metadata|bonus|dnode|dbuf|hdr|l2_hdr)_size ' $AS
echo ""
grep '^arc_meta_used ' $AS
echo -n 'COMPUTED                             '
egrep '^(metadata|bonus|dnode|dbuf|hdr|l2_hdr)_size ' $AS | awk 'END {print total} {total+=$3}'
