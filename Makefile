CFLAGS=-pipe -O2 -s
DESTDIR=$(HOME)/bin
ALL= addcr antiuniq cdcalc chunkit statfile unsh addcr

all: $(ALL)
	@echo done

install: all
	cp $(ALL) $(DESTDIR)

#	for i in $(ALL); do strip $(DESTDIR)/$$i; done

clean:
	rm -f $(ALL)
