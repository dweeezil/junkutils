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
 */

main(int argc, char **argv) {
	int megs, incrmeg = 1, stopmeg = 0;
	char *x;
	intptr_t incr;

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
		memset(x, 0x55, incr);
		if (stopmeg && megs >= stopmeg)
			break;
	}
	printf("%d MiB (%d GiB)\n", megs, megs / 1024);
}
