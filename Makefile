CFLAGS=-pipe -O2 -s
DESTDIR=$(HOME)/bin
ALL= addcr antiuniq cdcalc chunkit hogmem statfile unsh addcr maxsbrk

all: $(ALL)
	@echo done

install: all
	cp $(ALL) $(DESTDIR)

clean:
	rm -f $(ALL) *.o

hogmem: hogmem.o hogmem1.o
	$(CC) $(LDFLAGS) hogmem.o hogmem1.o -o hogmem
