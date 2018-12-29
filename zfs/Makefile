CFLAGS=-pipe -O2 -s
DESTDIR=$(HOME)/bin
ALL= raidzdump zio-pipeline

all: $(ALL)
	@echo done

raidzdump: raidzdump.c
	$(CC) $(CFLAGS) -I../../zfs/lib/libspl/include raidzdump.c -o raidzdump

zio-pipeline: zio-pipeline.c
	$(CC) $(CFLAGS) zio-pipeline.c -o zio-pipeline

clean:
	rm -f $(ALL)
