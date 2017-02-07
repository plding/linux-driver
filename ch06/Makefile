KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
TEST_PROGS = globalmem_test globalmem2_test

obj-m += globalmem.o globalmem2.o

all: modules tests

tests: $(TEST_PROGS)

globalmem_test : ../libtest.a globalmem_test.o
globalmem2_test: ../libtest.a globalmem2_test.o

.c.o:
	$(CC) -I../lib -c $^ -o $@

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -f $(TEST_PROGS)
