#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
main(int argc, char **argv) {
	int megs = 1;
	char *buf;
	size_t val;
	unsigned char c;

	if (argc > 1)
		megs = atoi(argv[1]);
	val = megs * 1048576UL;
	buf = malloc(val);
	if (!buf) {
		printf("no mem\n");
		exit(1);
	}
	for (c = 0 ; ; ++c) {
		memset(buf, c, val);
		putchar("-\\|/"[c % 4]);
		fflush(stdout);
		putchar('\b');
	}
}
