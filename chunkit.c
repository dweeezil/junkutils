#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/* default is for 80 minute CD-R (75 sectors/second)
 */
#define DEFMAX (80L * 60L * 75L * 2048L)

main(int ac, char **av) {
	char buf[512], buf1[512];
	struct stat s;
#ifdef __linux
	long long i = 0, MAX;
#else
	off_t i = 0, MAX;
#endif
	int counter = 0;
	FILE *fp = (FILE *)0;

	if (ac == 2)
		MAX = strtol(av[1], 0, 10) * 1000LL * 1000LL;
	else
		MAX = DEFMAX;

	while (fgets(buf, sizeof buf, stdin)) {
		buf[strlen(buf) - 1] = '\0';
		if (lstat(buf, &s))
			continue;
		if (s.st_size > MAX) {
			fprintf(stderr, "file \"%s\" is too large, aborting.\n", buf);
			exit(1);
		}
		if (strchr(buf, '=')) {
			fprintf(stderr, "WARN: skipping \"%s\" - contains '='\n", buf);
			continue;
		}
		i += s.st_size;
	redo:
		if (fp == (FILE *)0) {
			sprintf(buf1, "chunkit.%2.2d", counter++);
			if ((fp = fopen(buf1, "w")) == (FILE *)0) {
				fprintf(stderr, "can't create \"%s\"\n", buf1);
				exit(1);
			}
		}
		if (i > MAX) {
			if (fclose(fp)) {
				fprintf(stderr, "error closing file\n");
				exit(1);
			}
			i = s.st_size;
			fp = (FILE *)0;
			goto redo;
		}
		fprintf(fp, "%s=%s\n", buf, buf);
	}
}
