#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void doescape(char *s) {
	int i;
	while ((i = *s) != '\0') {
		if (isalnum(i) || strchr("-_.!~*'()", i) != NULL)
			putchar(i);
		else
			printf("%%%2.2x", i);
		++s;
	}
}

int
main(int ac, char **av) {
	for (int i = 1; i < ac ; ++i) {
		char *s = av[i];
		char *t = strcasestr(s, "://");

		if (t) {
			while (s < t) {
				putchar(*s);
				++s;
			}
			fputs("://", stdout);
			doescape(s + 3);
		} else {
			doescape(s);
		}
		putchar('\n');
	}
}
