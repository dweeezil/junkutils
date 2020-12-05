CFLAGS=-pipe -O2 -s
DESTDIR=$(HOME)/bin
ALL= addcr antiuniq cdcalc chunkit statfile unsh addcr maxsbrk

all: $(ALL)
	@echo done

install: all
	cp $(ALL) $(DESTDIR)

clean:
	rm -f $(ALL)
