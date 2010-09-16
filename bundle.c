#include <stdio.h>

/*	bundle - Concatenate consecutive nonblank input lines into single
 *	lines separated by either a space or a separator character.
 */


//	doline - copy a line from stdin to stdout as a single output field.
//	If we're "in" a bundle as indicated by the "inbundle" flag, prepend
//	a separator character to the field.
//
doline(int inbundle, int sep) {
	int i, rval = 0;

	while ((i = getchar()) != EOF) {
		if (i == '\n')
			return rval;
		if (inbundle && sep) {
			putchar(sep);
			sep = 0;
		}
		putchar(i);
		++rval;
	}
	return rval;
}

main(int argc, char **argv) {
	int sep;		// field seperator
	int inbundle = 0;	// nonzero if we're "in" a bundle (i.e. have read one field of a bundle)
	int lastlinelen = -1;	// length of previous input line
	int thislinelen;	// length of current input line

	if (argc == 2)
		sep = argv[1][0];

	while (!feof(stdin)) {
		if (inbundle && lastlinelen == 0) {
			putchar('\n');
			inbundle = 0;
		}
		thislinelen = doline(inbundle, sep);
		if (thislinelen > 0)
			inbundle = 1;
		lastlinelen = thislinelen;
	}
	if (inbundle)
		putchar('\n');
}
