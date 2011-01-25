#include <stdio.h>
#include <stdlib.h>

main(int ac, char **av) {
	FILE *infile, *outfile;
	int i, outfilefd;
	char *name, *temp;

	while (name = *++av) {
		if (temp = malloc(strlen(name) + 7)) {
			if ((infile = fopen(name, "r")) == (FILE *)0) {
				fprintf(stderr, "addcr: can't open \"%s\" for reading\n");
				exit(1);
			}
			strcpy(temp, name);
			strcat(temp, "XXXXXX");
			if ((outfilefd = mkstemp(temp)) == -1
			 || (outfile = fdopen(outfilefd, "w")) == (FILE *)0) {
				fprintf(stderr, "addcr: can't create temporary file\n");
				exit(1);
			}
		} else {
			fprintf(stderr, "addcr: out of memory\n");
			exit(1);
		}
		while ((i = getc(infile)) != EOF) {
			if (i == '\n')
				putc('\r', outfile);
			putc(i, outfile);
		}
		if (ferror(infile) || (fclose(infile) == EOF)) {
			fprintf(stderr, "addcr: read error\n");
			unlink(temp);
			exit(1);
		}
		if (ferror(outfile) || (fclose(outfile) == EOF)) {
			fprintf(stderr, "addcr: write error\n");
			unlink(temp);
			exit(1);
		}
		if (rename(temp, name) == -1) {
			fprintf(stderr, "addcr: rename error\n");
			unlink (temp);
			exit(1);
		}
		unlink(temp);
	}
}
