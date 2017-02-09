KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
TEST_PROGS := globalfifo_poll_test

obj-m += globalfifo.o

all: modules tests

tests: $(TEST_PROGS)

%_test: %_test.o
	$(CC) -o $@ $^ ../libtest.a

.c.o:
	$(CC) -I../lib -c $^ -o $@

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -f $(TEST_PROGS)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
