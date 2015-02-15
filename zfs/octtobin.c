#include <stdio.h>
#include <stdlib.h>

abend(char *s) {
	fprintf(stderr, "%s\n", s);
	exit(1);
}

main() {
	int i0, i1, i2, i3;
	int x;

	for (;;) {
		i0 = getchar();
		i1 = getchar();
		i2 = getchar();
		i3 = getchar();
		if (feof(stdin))
			exit(0);
		if (i0 != '\\')
			abend("backslash");
		x = (i1 - '0') << 6;
		x |= (i2 - '0') << 3;
		x |= i3 - '0';
		putchar(x);
	}
}
