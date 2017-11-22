
include Make.defines

PROGS = globalmem_read globalmem_write

all: $(LIBNAME) $(PROGS)

globalmem_read: globalmem_read.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalmem_write: globalmem_write.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

$(LIBNAME):
	$(MAKE) -C lib

clean:
	rm -f $(PROGS) $(CLEANFILES) 
