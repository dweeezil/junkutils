#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void yabba(int n, char *buf) {
	sprintf(buf, "%d.%d.%d.%d",
		(n >> 24) & 0xffL,
		(n >> 16) & 0xffL,
		(n >> 8) & 0xffL,
		n & 0xffL);
}

void dabba(int n, char *buf) {
	sprintf(buf, "%2.2X%2.2X%2.2X%2.2X",
		(n >> 24) & 0xffL,
		(n >> 16) & 0xffL,
		(n >> 8) & 0xffL,
		n & 0xffL);
}

main(int ac, char **av) {
	int l, m, max;
	signed int mask = 0x80000000;
	char buf[50];
	int i, skip = 1;
	void (*func)(int, char *) = yabba;

	if (ac != 3 && ac != 4) {
		fprintf(stderr, "usage: makerange <IP address> <mask length> [<skip>]\n");
		fprintf(stderr, "example: makerange 206.190.30.24 30\n");
		exit(1);
	}
	if (ac == 4)
		skip = atoi(av[3]);

	if (av[0][strlen(av[0]) - 1] == 'x')
		func = dabba;

	l = ntohl(inet_addr(av[1]));
	m = atoi(av[2]);
	mask >>= (m < 0 ? -m : m) - 1;
	l &= mask;
	max = 0xffffffff & ~mask;
	if (m < 0)
		--max;

	for (i = m < 0 ? 1 : 0 ; i <= max ; i += skip) {
		func(l | i, buf);
		printf("%s\n", buf);
	}
	exit(0);
}
