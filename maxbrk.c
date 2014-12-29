#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MEG (1024ULL * 1024ULL)

main(int argc, char **argv) {
	int megs, incrmeg;
	char *startbrk, *testbrk;
	unsigned long long incr;

	startbrk = sbrk(0);
	for (incr = 2 * MEG ; incr > MEG ; incr += MEG) {
		if (brk(startbrk + incr) == -1)
			break;
	}
	(void) brk(startbrk);
	printf("%llu MiB (%llu GiB)\n", incr / MEG, incr / (MEG * 1024ULL));
}
