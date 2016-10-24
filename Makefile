objs=${prog}.o util.o widget.o main.o
include config.mk
CFLAGS+=-std=c99
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=-D_DEFAULT_SOURCE
CFLAGS+=-D_BSD_SOURCE
LDFLAGS+=-Llibjb
LDFLAGS+=-lxcb -ljb
PREFIX=/usr
prog=xstatus
installdir=${DESTDIR}${PREFIX}
all:
	cd libjb && ${MAKE} CC="${CC}" CFLAGS="${CFLAGS}"
	make ${prog}
	ls -l ${prog} >> sz.log; tail -n 5 sz.log
include depend.mk
${objs}: *.h
${prog}: ${objs} libjb/libjb.a
	${CC} ${CFLAGS} ${objs} libjb/libjb.a -o $@ ${LDFLAGS}
clean:
	rm -f ${prog} *.o
	cd libjb && make clean
install:
	install -d ${installdir}/bin
	install -s xstatus ${installdir}/bin
depend:
	${CC} -E -MM *.c > depend.mk
#EOF
