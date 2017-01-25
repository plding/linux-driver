DIRS = ch04

modules:
	@for dir in ${DIRS}; do ${MAKE} -C $${dir} modules; done

clean:
	@for dir in ${DIRS}; do ${MAKE} -C $${dir} clean; done
