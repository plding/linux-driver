
include Make.defines

PROGS = globalmem_test globalmem_read globalmem_write globalmem_clr \
		globalmem_concurrency \
		globalfifo_read globalfifo_write \
		globalfifo_select globalfifo_poll globalfifo_epoll

all: $(LIBNAME) $(PROGS)

globalmem_test: globalmem_test.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalmem_read: globalmem_read.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalmem_write: globalmem_write.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalmem_clr: globalmem_clr.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalmem_concurrency: globalmem_concurrency.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalfifo_read: globalfifo_read.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalfifo_write: globalfifo_write.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalfifo_select: globalfifo_select.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalfifo_poll: globalfifo_poll.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

globalfifo_epoll: globalfifo_epoll.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

$(LIBNAME):
	$(MAKE) -C lib

clean:
	rm -f $(PROGS) $(CLEANFILES) 
