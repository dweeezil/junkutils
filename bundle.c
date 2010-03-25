#include <stdio.h>

/* bundle - "Bundle" groups of lines in the input by joining together
 * consecutive lines separated by blank lines.
 */

main() {
	int i, len = 0;

	while ((i = getchar()) != EOF) {

		/* On a non-newline, write the character and increment the
		 * line length counter.
		 */
		if (i != '\n') {
			putchar(i);
			++len;

		/* Got a newline...
		 */
		} else {
			if (len == 0)		/* separate bundles */
				putchar('\n');
			else {
				putchar(' ');	/* separate lines */
				len = 0;	/* new input line */
			}
		}
	}
	if (len)
		putchar('\n');
}
