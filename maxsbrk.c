#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

main(int argc, char **argv) {
	int megs, incrmeg, stopmeg;
	char *x;
	intptr_t incr;

	if (argc == 2)
		incrmeg = atoi(argv[1]);
	else
		incrmeg = 1;

	if (argc == 3)
		stopmeg = atoi(argv[2]);
	else
		stopmeg = 0;

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
