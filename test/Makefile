
include Make.defines

PROGS = globalmem_test globalmem_read globalmem_write globalmem_clr

all: $(LIBNAME) $(PROGS)

globalmem_test: globalmem_test.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalmem_read: globalmem_read.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalmem_write: globalmem_write.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalmem_clr: globalmem_clr.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

$(LIBNAME):
	$(MAKE) -C lib

clean:
	rm -f $(PROGS) $(CLEANFILES) 
