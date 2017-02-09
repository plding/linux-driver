DIRS = lib \
	   ch04 \
	   ch06 \
	   ch08

all:
	@for dir in ${DIRS}; do ${MAKE} -C $${dir} all; done

clean:
	@for dir in ${DIRS}; do ${MAKE} -C $${dir} clean; done
