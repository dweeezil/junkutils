#include <unistd.h>
#include <stdlib.h>

main(int argc, char **argv) {
	int delay = 1;
	char buf[10];
	int i;

	if (argc == 2)
		delay = atoi(argv[1]);

	for (;;) {
		i = read(0, buf, 1);
		if (i < 0)
			exit(0);
		sleep(delay);
	}
}
