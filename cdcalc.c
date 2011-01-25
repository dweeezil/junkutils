#include <stdio.h>
#include <sys/stat.h>
#include <math.h>

main() {
	char buf[512];
	struct stat s;
	struct tm *t;
	long x = 0;
	double a, b, c;

	while (fgets(buf, sizeof buf, stdin)) {
		buf[strlen(buf) - 1] = '\0';
		if (lstat(buf, &s) != -1)
			x += s.st_size;
	}
	a = (double)x / (44100.0 * 16.0 * 2.0 / 8.0 * 60.0);
	b = modf(a, &c);
	printf("%d:%2.2d\n", (int)c, (int)(b * 60.0));
}
