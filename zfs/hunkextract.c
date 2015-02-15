#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main() {
	char buf[10240];
	int inpattern = 0;

	while (fgets(buf, sizeof buf, stdin)) {
		if (strncmp(buf, "@@", 2) == 0) {
			if (inpattern)
				exit(0);
			inpattern = 1;
			continue;
		}
		if (inpattern)
			fputs(buf, stdout);
	}
}
