#include <stdio.h>

/*
 *  unldif - fold continuation lines (space following a newlin) in an LDIF file
 */

main() {
	int c;
	int state = 0;


	while ((c = getchar()) != EOF) {
		switch (state) {
		case 0:				/* initial */
			if (c == '\n') {
				state = 1;
			} else if (c != EOF) {
				putchar(c);
			}
			break;
		case 1:				/* seen "\n" */
			if (c == ' ') {
				state = 2;
			} else if (c == EOF) {
				putchar('\n');
			} else {
				putchar('\n');
				putchar(c);
				state = 0;
			}
			break;
		case 2:				/* seen "\n " */
			if (c != EOF)
				putchar(c);
			state = 0;
			break;
		}
	}
}
