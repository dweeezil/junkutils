#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

char *
myfgets(char *s, int n, FILE *fp) {
	int i, j;

	for (--n, j = 0 ; j < n ; ++j) {
		if ((i = getc(fp)) == EOF) {
			if (j == 0) {
				*s = '\0';
				return (char *)0;
			}
			break;
		}
		if (i == '\n')
			break;
		*s++ = i;
	}
	*s = '\0';
	return s;
}

main() {
	char buf[512];
	struct stat s;
	struct tm *t;

	while (myfgets(buf, sizeof buf, stdin)) {
		if (lstat(buf, &s) != -1) {
			t = gmtime(&s.st_mtime);
			printf("%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d %lld 0%o %s\n",
				1900 + t->tm_year,
				t->tm_mon + 1,
				t->tm_mday,
				t->tm_hour,
				t->tm_min,
				t->tm_sec,
				(long long)s.st_mtime,
				s.st_mode,
				buf);
		}
	}
}
