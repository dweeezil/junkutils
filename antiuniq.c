#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX 65536

char matchbuf[MAX];
char buf[MAX];

int firstcmp, lastcmp;

docmp(char *s1, char *s2) {
	int len1, len2, f, l;

	if (firstcmp == -1 || lastcmp == -1)
		return (strcmp(s1, s2));

	len1 = strlen(s1);
	len2 = strlen(s2);
	if (len1 > len2)
		len1 = len2;
	if (firstcmp > len1)
		f = len1;
	else
		f = firstcmp;
	if (lastcmp > len1)
		l = len1;
	else
		l = lastcmp;
	return (memcmp(s1 + f - 1, s2 + f - 1, l - f + 1));
}

main(int ac, char **av) {
	char *name;
	FILE *fp;
	int firstflag;		/* nonzero if working on the first line */
	int matchout;		/* count of identical lines output so far */
	int ch;
	int sflag = 0;		/* nonzero to print out single lines */

	firstcmp = lastcmp = -1;
	while ((ch = getopt(ac, av, "sf:l:")) != -1)
		switch (ch) {
			case 'f':
				firstcmp = atoi(optarg);
				break;
			case 'l':
				lastcmp = atoi(optarg);
				break;
			case 's':
				sflag = 1;
				break;
			case '?':
			default:
				fprintf(stderr, "usage: antiuniq [-s] [-f <first>] [-l <last>] file ...\n");
				exit(1);
		}
	if (firstcmp > 0 && lastcmp > 0 && firstcmp > lastcmp) {
		fprintf(stderr, "antiuniq: %d is > %d\n", firstcmp, lastcmp);
		exit(1);
	}
	ac -= optind;
	av += optind;
	while (name = *av++) {
		if ((fp = fopen(name, "r")) == (FILE *)0) {
			fprintf(stderr, "antiuniq: can't open \"%s\"\n", name);
			continue;
		}
#ifndef __linux
		setproctitle("processing - %s", name);
#endif
		buf[0] = matchbuf[0] = '\0';
		firstflag = 1;
		matchout = 0;
		while (fgets(buf, sizeof buf, fp)) {
			if (matchbuf[0] == '\0') {
				strcpy(matchbuf, buf);
			} else if (docmp(matchbuf, buf) == 0) {
				fputs(buf, stdout);
				++matchout;
			} else {
				if (matchout == 0 && sflag)
					fputs(buf, stdout);
				strcpy(matchbuf, buf);
				matchout = 0;
			}
			firstflag = 0;
		}
		if (matchout == 0 && sflag)
			fputs(buf, stdout);
		fclose(fp);
	}
	exit(0);
}
