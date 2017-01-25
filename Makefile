DIRS = ch04 ch06

modules:
	@for dir in ${DIRS}; do ${MAKE} -C $${dir} modules; done

clean:
	@for dir in ${DIRS}; do ${MAKE} -C $${dir} clean; done
