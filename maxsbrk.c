#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * maxsbrk - Determine how much memory can be allocated and dirtied.
 *
 * Usage: maxsbrk <alloc> [<chunk>]
 *
 * alloc - Amount of memory to allocate in MiB.  If specified as zero (0),
 * memory is allocated until the attempts to do so fail.
 *
 * chunk - Allocate memory in chunks of this size in MiB.  The default chunk
 * size is 1MiB.
 *
 * If invoked as an executable containing the string "hog", continue to
 * dirty memory until interrupted.
 */

int
main(int argc, char **argv) {
	int megs, chunksize, allocsize, i;
	char *x, *base = NULL;
	intptr_t incr;
	int hog = 0;

	if (strstr(argv[0], "hog") != NULL)
		hog = 1;

	switch (argc) {
	case 2:
		allocsize = atoi(argv[1]);
		chunksize = 1;
		break;
	case 3:
		allocsize = atoi(argv[1]);
		chunksize = atoi(argv[2]);
		break;
	default:
		fprintf(stderr, "usage: %s 0|<alloc MiB> [<chunk MiB>]\n", argv[0]);
		exit(1);
		break;
	}

	incr = chunksize * 1024ULL * 1024ULL;

	for (megs = 0 ; ; megs += chunksize) {
		x = sbrk(incr);
		if (x == (void *)-1) {
			printf("sbrk failed\n");
			break;
		}
		if (base == NULL)
			base = x;
		memset(x, 0x55, incr);
		if (allocsize && megs >= allocsize)
			break;
	}
	printf("%d MiB (%.1f GiB)\n", megs, (double)megs / 1024.0);

	if (hog) {
		printf("Hog mode enabled, continuing to dirty memory.\n");

		for (;; ++hog)
			for (i = 0, x = base; i < megs ; ++i, x += chunksize)
				memset(x, hog, incr);
	}
}
