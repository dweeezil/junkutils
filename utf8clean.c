/*
 *	utf8clean - drop bogus UTF8 sequences
 */

#include <stdio.h>
#include <stdlib.h>

void
err(char *s, int st, long long l) {
	fprintf(stderr, "%s[%d] %lld\n", s, st, l);
}
#define ERR err(*av, state, offset)

int
main(int ac, char **av)	{
	FILE *fp;
	int i;
	int seq[4];					/* 4 possible bytes of a sequence */
	int state;					/* # bytes of a sequence seen so far */
	long long offset;			/* offset in file */
	long twos, threes, fours;

	while (*++av) {
		if ((fp = fopen(*av, "r")) == (FILE *)0) {
			fprintf(stderr, "can't open \"%s\"\n", *av); fflush(stderr);
			continue;
		}
		state = 0;
		offset = 0;
		twos = threes = fours = 0;
		while ((i = getc(fp)) != EOF) {
			switch (state) {
				case 0:
					if (i >= 0 && i <= 0x7f)
						putchar(i);
					else if (i != 0xc0 && i != 0xc1 && i <= 0xf4) {
						state = 1;
						seq[0] = i;
					} else {
						ERR;
						/* drop character */
					}
					break;
				case 1:
					if (seq[0] >= 0xc2 && seq[0] <= 0xdf
					 && i >= 0x80 && i <= 0xbf) {	/* good 2-byte sequence, output it */
						++twos;
						putchar(seq[0]);
						putchar(i);
						state = 0;
					} else if (seq[0] == 0xe0 && i >= 0xa0 && i <= 0xbf
						  || seq[0] >= 0xe1 && seq[0] <= 0xec && i >= 0x80 && i <= 0xbf
						  || seq[0] == 0xed && i >= 0x80 && i <= 0x9f
						  || seq[0] == 0xee && i >= 0x80 && i <= 0xbf
						  || seq[0] == 0xef && i >= 0x80 && i <= 0xbf
						  || seq[0] == 0xf0 && i >= 0x90 && i <= 0xbf
						  || seq[0] >= 0xf1 && seq[0] <= 0xf3 && i >= 0x80 && i <= 0xbf
						  || seq[0] == 0xf4 && i >= 0x80 && i <= 0x8f) {	/* 1st two bytes of 3 or 4 byte sequence */
						state = 2;
						seq[1] = i;
					} else {
						/* drop both characters */
						ERR;
						state = 0;
					}
					break;
				case 2:
					if (i >= 0x80 && i <= 0xbf)	{	/* good 3rd byte, either 3 or 4-byte sequence */
						if (seq[0] >= 0xf0) {		/* either 4 byte sequence or bogus */
							seq[2] = i;
							state = 3;
						} else {					/* good 3-byte sequence */
							++threes;
							putchar(seq[0]);
							putchar(seq[1]);
							putchar(i);
							state = 0;
						}
					} else {
						/* drop 3 characters */
						ERR;
						state = 0;
					}
					break;
				case 3:
					if (i >= 0x80 && i <= 0xbf) {	/* valid 4-byte sequence */
						++fours;
						putchar(seq[0]);
						putchar(seq[1]);
						putchar(seq[2]);
						putchar(i);
						state = 0;
					} else {
						/* drop 4 characters */
						ERR;
						state = 0;
					}
					break;
			}
			++offset;
		}
		if (twos > 0L || threes > 0L || fours > 0L) {
			fprintf(stderr, "%s: 2:%ld 3:%ld 4:%ld\n", *av, twos, threes, fours);
			fflush(stdout);
		}
		fclose(fp);
	}
	exit(0);
}
