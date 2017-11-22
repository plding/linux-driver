
include ../Make.defines

all:
	$(CC) -c $(CFLAGS) *.c
	rm -f ../$(LIBNAME)
	ar rs ../$(LIBNAME) *.o

clean:
	rm -f ../$(LIBNAME) *.o
