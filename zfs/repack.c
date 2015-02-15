#include <stdio.h>
#include <stdlib.h>

void
writeull(unsigned long long x) {
	int i;

	for (i = 0 ; i <= 56 ; i += 8)
	/* for (i = 56 ; i >= 0 ; i -= 8) */
		putchar(x >> i);
}

main() {
	char buf1[128];
	char buf2[128];
	unsigned long long x;

	buf1[0] = '\0';
	buf2[0] = '\0';
	while (scanf("%s %s", buf1, buf2) == 2) {
		x = strtoull(buf1, NULL, 16);
		writeull(x);
		x = strtoull(buf2, NULL, 16);
		writeull(x);
		buf1[0] = '\0';
		buf2[0] = '\0';
	}
}
