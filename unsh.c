/* unsh - escape special shell characters with a backslash
 */

#include <stdio.h>
#include <string.h>

main() {
	int i;

	while ((i = getchar()) != EOF) {
		if (strchr(" $&*(){[}]\\|;'\"<>?", i))
			putchar('\\');
		putchar(i);
	}
}
