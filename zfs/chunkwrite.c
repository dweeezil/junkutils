/* issue #1297 */

#include "stdio.h"
#define _XOPEN_SOURCE 500
#include "unistd.h"
#include "fcntl.h"

#include "malloc.h"

// 1 megabyte for sake of example, read from /dev/urandom so it's not compressible
#define BLOCKSIZE 1048576

// Select a size much smaller than the amount of
// RAM you have, but still big enough to be significant
// (eg: 80 megabytes for a blocksize of 1M)
#define CHUNKS 80

// App should run for a while, but not forever
#define ITERATIONS 512

// Whether you use write or pwrite doesn't seem to make a difference, but it's what BDB uses.
/* #define PWRITE */

int main() {
  int fd;
  int len;
  int i, j;

  char *data = malloc(BLOCKSIZE);
  if (!data)
    return 1;

  fd = open("/dev/urandom", O_RDONLY); // uncompressable content
  if (fd < 0)
    return 2;
  len = read(fd, data, BLOCKSIZE);
  if (len != BLOCKSIZE)
    return 3;
  close(fd);

  fd = open("testfile", O_RDWR | O_CREAT, 0644);
  if (fd < 0)
    return 4;

  for (i=0; i < ITERATIONS; i++) {
    for (j=0; j < CHUNKS ; j++) {
#ifdef PWRITE
      pwrite(fd, data, BLOCKSIZE, BLOCKSIZE * j);
#else
      write(fd, data, BLOCKSIZE);
#endif
      printf("Chunk %d/%d\n", j, CHUNKS);
    }
    printf("Lap %d/%d\n", i, ITERATIONS);
    lseek(fd, 0, 0); // back to start (if using regular write, harmless otherwise);
  }

  close(fd);
  return 0;

}
