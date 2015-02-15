#include <stdio.h>
#include <stdlib.h>
#include <libnvpair.h>

void abend(char *s) {
	fprintf(stderr, "dumpsa: %s\n", s);
	exit(1);
}

unsigned long
get32le() {
	unsigned long x;

	x = getchar();
	x |= getchar() << 8;
	x |= getchar() << 16;
	x |= getchar() << 24;
	if (feof(stdin))
		abend("get32le: premature end of file");
	return x;
}

unsigned long
get16le() {
	unsigned long x;

	x = getchar();
	x |= getchar() << 8;
	if (feof(stdin))
		abend("get16le: premature end of file");
	return x;
}

static void
dump_znode_sa_xattr(char *sa_xattr_packed, int sa_xattr_size)
{
	nvlist_t *sa_xattr;
	nvpair_t *elem = NULL;
	int sa_xattr_entries = 0;
	int error;

	error = nvlist_unpack(sa_xattr_packed, sa_xattr_size, &sa_xattr, 0);
	if (error)
		return;

	while ((elem = nvlist_next_nvpair(sa_xattr, elem)) != NULL)
		sa_xattr_entries++;

	(void) printf("\tSA xattrs: %d bytes, %d entries\n\n",
	    sa_xattr_size, sa_xattr_entries);
	while ((elem = nvlist_next_nvpair(sa_xattr, elem)) != NULL) {
		uchar_t *value;
		uint_t cnt, idx;

		(void) printf("\t\t%s = ", nvpair_name(elem));
		nvpair_value_byte_array(elem, &value, &cnt);
		for (idx = 0; idx < cnt; ++idx) {
			if (isprint(value[idx]))
				(void) putchar(value[idx]);
			else
				(void) printf("\\%3.3o", value[idx]);
		}
		(void) putchar('\n');
	}
}

main(int argc, char **argv) {
	unsigned long x;
	unsigned long magic, hdrsize, layout, sasize;
	unsigned char *buf, *s;
	int i;

	magic = get32le();
	printf("magic: 0x%lx\n", magic);

	x = get16le();
	layout = x & 0x3f;
	hdrsize = (x >> 10) << 3;
	printf("hdrsize: %lu\n", hdrsize);
	printf("layout: %lu\n", layout);

	sasize = get16le();
	printf("SA size: %lu\n", sasize);

	buf = malloc(65536);
	/* buf = malloc(sasize); */
	if (!buf)
		abend("no memory");
	for (s = buf; ; ++s) {
		i = getchar();
		if (i == EOF)
			break;
		*s = i;
	}

	dump_znode_sa_xattr(buf, sasize);
}
