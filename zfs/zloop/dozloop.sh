#!/bin/sh
#ZFS_DEBUG=on /usr/share/zfs/zloop.sh -t 3600 -- -o zio_deadman_log_all=1 -o zfs_deadman_ziotime_ms=10000 -o zfs_deadman_synctime_ms=30000 -o zfs_deadman_checktime_ms=10000
ZFS_DEBUG=on /usr/share/zfs/zloop.sh -t 3600


# options zfs zfs_deadman_checktime_ms=10000
# options zfs zfs_deadman_synctime_ms=30000
# options zfs zfs_deadman_ziotime_ms=10000
# #options zfs zfs_deadman_failmode=continue
# #options zfs zfs_flags=4294966271
# 
# #options zfs zfs_vdev_aggregation_limit=0
# 
# options spl spl_kmem_cache_slab_limit=0
