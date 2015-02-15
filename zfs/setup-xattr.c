#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

main() {
	int fd;
	static char filename[] = "UPS.txt";
	static char xattrname[] = "user.rsync.%stat";
	static char xattrval[] = "100650 0,0 43874288:43874288";
	struct timespec t[2];

	fd = open(filename, O_WRONLY | O_CREAT, 0600);
	close(fd);
	lsetxattr(filename, xattrname, xattrval, sizeof xattrval - 1, 0);

#if 0
	t[0].tv_sec = 0;
	t[0].tv_nsec = UTIME_NOW;
	t[1].tv_sec = 1295479844;
	t[1].tv_nsec = 0;
	utimensat(AT_FDCWD, filename, t, AT_SYMLINK_NOFOLLOW);
	lchown(filename, 1079, 1000);
#endif
	chmod(filename, 0650);
}
