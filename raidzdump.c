/*
 * Given an offset, size, number of disks in the raidz pool,
 * the number of parity "disks" (1, 2, or 3 for raidz, raidz2, raidz3),
 * and the sector size (shift),
 * print a set of stripes.
 */

#include <sys/types.h>
#include <sys/sysmacros.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

/*
 * The following are taken straight from usr/src/uts/common/fs/zfs/vdev_raidz.c
 * If they change there, they need to be changed here.
 *
 * a map of columns returned for a given offset and size
 */
typedef struct raidz_col {
    uint64_t rc_devidx;     /* child device index for I/O */
    uint64_t rc_offset;     /* device offset */
    uint64_t rc_size;       /* I/O size */
    void *rc_data;          /* I/O data */
    void *rc_gdata;         /* used to store the "good" version */
    int rc_error;           /* I/O error for this device */
    uint8_t rc_tried;       /* Did we attempt this I/O column? */
    uint8_t rc_skipped;     /* Did we skip this I/O column? */
} raidz_col_t;

typedef struct raidz_map {
    uint64_t rm_cols;       /* Regular column count */
    uint64_t rm_scols;      /* Count including skipped columns */
    uint64_t rm_bigcols;        /* Number of oversized columns */
    uint64_t rm_asize;      /* Actual total I/O size */
    uint64_t rm_missingdata;    /* Count of missing data devices */
    uint64_t rm_missingparity;  /* Count of missing parity devices */
    uint64_t rm_firstdatacol;   /* First data column/parity count */
    uint64_t rm_nskip;      /* Skipped sectors for padding */
    uint64_t rm_skipstart;  /* Column index of padding start */
    void *rm_datacopy;      /* rm_asize-buffer of copied data */
    uintptr_t rm_reports;       /* # of referencing checksum reports */
    uint8_t rm_freed;       /* map no longer has referencing ZIO */
    uint8_t rm_ecksuminjected;  /* checksum error was injected */
    raidz_col_t rm_col[1];      /* Flexible array of I/O columns */
} raidz_map_t;

/*
 *  vdev_raidz_map_get() is hacked from vdev_raidz_map_alloc() in
 *  usr/src/uts/common/fs/zfs/vdev_raidz.c.  If that routine changes,
 *  this might also need changing.
 */

raidz_map_t *
vdev_raidz_map_get(uint64_t size, uint64_t offset, uint64_t unit_shift, 
           uint64_t dcols, uint64_t nparity)
{
    raidz_map_t *rm;
    uint64_t b = offset >> unit_shift;
    uint64_t s = size >> unit_shift;
    uint64_t f = b % dcols;
    uint64_t o = (b / dcols) << unit_shift;
    uint64_t q, r, c, bc, col, acols, scols, coff, devidx, asize, tot;

    q = s / (dcols - nparity);
    r = s - q * (dcols - nparity);
    bc = (r == 0 ? 0 : r + nparity);
    tot = s + nparity * (q + (r == 0 ? 0 : 1));

    if (q == 0) {
        acols = bc;
        scols = MIN(dcols, roundup(bc, nparity + 1));
    } else {
        acols = dcols;
        scols = dcols;
    }

    rm = malloc(offsetof(raidz_map_t, rm_col[scols]));

    if (rm == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    rm->rm_cols = acols;
    rm->rm_scols = scols;
    rm->rm_bigcols = bc;
    rm->rm_skipstart = bc;
    rm->rm_missingdata = 0;
    rm->rm_missingparity = 0;
    rm->rm_firstdatacol = nparity;
    rm->rm_datacopy = NULL;
    rm->rm_reports = 0;
    rm->rm_freed = 0;
    rm->rm_ecksuminjected = 0;

    asize = 0;

    for (c = 0; c < scols; c++) {
        col = f + c;
        coff = o;
        if (col >= dcols) {
            col -= dcols;
            coff += 1ULL << unit_shift;
        }
        rm->rm_col[c].rc_devidx = col;
        rm->rm_col[c].rc_offset = coff;
        rm->rm_col[c].rc_data = NULL;
        rm->rm_col[c].rc_gdata = NULL;
        rm->rm_col[c].rc_error = 0;
        rm->rm_col[c].rc_tried = 0;
        rm->rm_col[c].rc_skipped = 0;

        if (c >= acols)
            rm->rm_col[c].rc_size = 0;
        else if (c < bc)
            rm->rm_col[c].rc_size = (q + 1) << unit_shift;
        else
            rm->rm_col[c].rc_size = q << unit_shift;

        asize += rm->rm_col[c].rc_size;
    }

    rm->rm_asize = roundup(asize, (nparity + 1) << unit_shift);
    rm->rm_nskip = roundup(tot, nparity + 1) - tot;

    if (rm->rm_firstdatacol == 1 && (offset & (1ULL << 20))) {
        devidx = rm->rm_col[0].rc_devidx;
        o = rm->rm_col[0].rc_offset;
        rm->rm_col[0].rc_devidx = rm->rm_col[1].rc_devidx;
        rm->rm_col[0].rc_offset = rm->rm_col[1].rc_offset;
        rm->rm_col[1].rc_devidx = devidx;
        rm->rm_col[1].rc_offset = o;

        if (rm->rm_skipstart == 0)
            rm->rm_skipstart = 1;
    }

    return (rm);

}

int
main(int argc, char *argv[])
{
    uint64_t offset = 0;
    uint64_t size = 0;
    uint64_t dcols = 0;
    uint64_t nparity = 1;
    uint64_t unit_shift = 9;  /* shouldn't be hard-coded.  sector size */
    raidz_map_t *rzm;
    raidz_col_t *cols;
    int i;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s offset size ndisks [nparity [ashift]]\n", argv[0]);
        fprintf(stderr, "  ndisks is number of disks in raid pool, including parity\n");
        fprintf(stderr, "  nparity defaults to 1 (raidz1)\n");
        fprintf(stderr, "  ashift defaults to 9 (512-byte sectors)\n");
        exit(1);
    }

    /* XXX - check return values */
    offset = strtoull(argv[1], NULL, 16);
    size = strtoull(argv[2], NULL, 16);
    dcols = strtoull(argv[3], NULL, 16);

    if (size == 0 || dcols == 0) { /* should check size multiple of ashift...*/
        fprintf(stderr, "size and/or number of columns must be > 0\n");
        exit(1);
    }

    if (argc > 4)
        nparity = strtoull(argv[4], NULL, 16);

    if (argc == 6)
        unit_shift = strtoull(argv[5], NULL, 16);

    rzm = vdev_raidz_map_get(size, offset, unit_shift, dcols, nparity);

    printf("cols = %llu, firstdatacol = %llu\n", (unsigned long long)rzm->rm_cols, (unsigned long long)rzm->rm_firstdatacol);
    for (i = 0, cols = &rzm->rm_col[0]; i < rzm->rm_cols; i++, cols++)
        printf("%llu:%lx:%lx\n", (unsigned long long)cols->rc_devidx, cols->rc_offset, cols->rc_size);

    exit(0);
}
