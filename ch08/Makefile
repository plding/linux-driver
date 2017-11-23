
KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := globalfifo.o globalfifo2.o \
		 globalfifo_1.o globalfifo2_1.o

all: modules $(PROGS)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
