CFLAGS=-pipe -O2 -s
DESTDIR=$(HOME)/bin
ALL= raidzdump

all: $(ALL)
	@echo done

raidzdump: raidzdump.c
	$(CC) $(CFLAGS) -I../../zfs/lib/libspl/include raidzdump.c -o raidzdump

clean:
	rm -f $(ALL)

