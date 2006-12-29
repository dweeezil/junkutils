#include <stdio.h>

/* unescape a postgres string */
/* states:
 * 0 - nothing
 * 1 - got backslash
 * 2 - got one character following a backslash
 * 3 - got two characters following a backslash
 */

main() {
	int state = 0;
	int i, octal;

	while ((i = getchar()) != EOF) {
		switch (state) {
			case 0:
				/* Initial state */
				if (i == '\\') {
					octal = 0;
					state = 1;
				} else {
					putchar(i);
				}
				break;
			case 1:
				/* Seen a backslash */
				switch (i) {
					case 'b': octal = '\b'; goto simplescape;
					case 'f': octal = '\f'; goto simplescape;
					case 'n': octal = '\n'; goto simplescape;
					case 'r': octal = '\r'; goto simplescape;
					case 't': octal = '\t';
					default:
					simplescape: putchar(octal); state = 0; continue;
					case '0': octal = '\000'; break;
					case '1': octal = '\100'; break;
					case '2': octal = '\200'; break;
					case '3': octal = '\300'; break;
				}
				state = 2;
				break;
			case 2:
				/* Seen a backslash and one digit */
				switch (i) {
					case '0': octal |= '\000'; break;
					case '1': octal |= '\010'; break;
					case '2': octal |= '\020'; break;
					case '3': octal |= '\030'; break;
					case '4': octal |= '\040'; break;
					case '5': octal |= '\050'; break;
					case '6': octal |= '\060'; break;
					case '7': octal |= '\070'; break;
					default: putchar(octal); state = 0; continue;
				}
				state = 3;
				break;
			case 3:
				/* Seen a backslash and two digits */
				switch (i) {
					case '0': octal |= '\000'; break;
					case '1': octal |= '\001'; break;
					case '2': octal |= '\002'; break;
					case '3': octal |= '\003'; break;
					case '4': octal |= '\004'; break;
					case '5': octal |= '\005'; break;
					case '6': octal |= '\006'; break;
					case '7': octal |= '\007'; break;
					default: putchar(octal); state = 0; continue;
				}
				putchar(octal);
				state = 0;
				break;
		}
	}
}
