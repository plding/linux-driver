CC := gcc
CFLAGS := -g -O2 -Wall -W
LIBNAME = ../libtest.a

all:
	${CC} -c ${CFLAGS} *.c
	rm -f ${LIBNAME}
	ar rs ${LIBNAME} *.o

clean:
	rm -f ${LIBNAME} *.o
