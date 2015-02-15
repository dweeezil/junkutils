#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#define FNAME "test-zfs-futimens.txt"

int main()
{
        struct stat st1;
        struct stat st2;

        unlink(FNAME);

        int fd = creat(FNAME, 0600);
        assert(fd >= 0);

        int e1 = fstat(fd, &st1);
        assert(e1 == 0);

        int e2 = futimens(fd, NULL);
        assert(e2 == 0);

        int e3 = fstat(fd, &st2);
        assert(e3 == 0);

        close(fd);
        unlink(FNAME);

        if (st2.st_mtim.tv_sec < st1.st_mtim.tv_sec || (st2.st_mtim.tv_sec == st1.st_mtim.tv_sec && st2.st_mtim.tv_nsec < st1.st_mtim.tv_nsec)) {
                printf("Error: st2 < st1\n\n");
                printf("st1: %lld.%.9ld\n", (long long)st1.st_mtim.tv_sec, st1.st_mtim.tv_nsec);
                printf("st2: %lld.%.9ld\n", (long long)st2.st_mtim.tv_sec, st2.st_mtim.tv_nsec);
                return 1;
        }

        printf("No error found.\n");

        return 0;
}
