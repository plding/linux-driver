KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := globalfifo.o globalfifo2.o globalfifo3.o

PROGS = globalfifo_read globalfifo_write \
		globalfifo_select globalfifo_poll globalfifo_epoll

all: modules $(PROGS)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -f $(PROGS)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
