CFLAGS=-pipe -O2 -s
DESTDIR=$(HOME)/bin
ALL= addcr antiuniq cdcalc chunkit statfile unsh addcr raidzdump

all: $(ALL)
	@echo done

install: all
	cp $(ALL) $(DESTDIR)

raidzdump: raidzdump.c
	$(CC) $(CFLAGS) -I../zfs/lib/libspl/include raidzdump.c -o raidzdump

clean:
	rm -f $(ALL)
