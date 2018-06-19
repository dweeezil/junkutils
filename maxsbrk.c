#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

/*
 * maxsbrk - Determine how much memory can be allocated and dirtied.
 *
 * Usage: maxsbrk [<chunk>] [<max>]
 *
 * The two optional arguments specify, respectively, the size in MiB of
 * the sbrk() allocations and the maximum amount of memory (in MiB)
 * allocated.  They are intended to be used to determine the effects
 * caused by a userland program which allocates large amounts of memory,
 * which is typically to cause the Linux kernel to engage its various
 * shrinkers in order that sufficient memory become available.
 *
 * If invoked as an executable containing the string "hog", continue to
 * dirty memory until interrupted.
 */

int
main(int argc, char **argv) {
	int megs, incrmeg = 1, stopmeg = 0, i;
	char *x, *base = NULL;
	intptr_t incr;
	int hog = 0;

	if (strstr(argv[0], "hog") != NULL)
		hog = 1;

	switch (argc) {
	case 2:
		incrmeg = atoi(argv[1]);
		break;
	case 3:
		incrmeg = atoi(argv[1]);
		stopmeg = atoi(argv[2]);
		break;
	}

	incr = incrmeg * 1024ULL * 1024ULL;

	for (megs = 0 ; ; megs += incrmeg) {
		x = sbrk(incr);
		if (x == (void *)-1) {
			printf("sbrk failed\n");
			break;
		}
		if (base == NULL)
			base = x;
		memset(x, 0x55, incr);
		if (stopmeg && megs >= stopmeg)
			break;
	}
	printf("%d MiB (%d GiB)\n", megs, megs / 1024);

	if (hog) {
		printf("Hog mode enabled, continuing to dirty memory.\n");

		for (;; ++hog)
			for (i = 0, x = base; i < megs ; ++i, x += incrmeg)
				memset(x, hog, incr);
	}
}
