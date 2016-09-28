CFLAGS+=-std=c11
LDFLAGS+=-lxcb
PREFIX=/usr
prog=xstatus
objs=${prog}.o util.o widget.o main.o
include config.mk
installdir=${DESTDIR}${PREFIX}

all:
	cd libjb && ${MAKE} CC="${CC}" CFLAGS="${CFLAGS}"
	make ${prog}
	ls -l ${prog} >> sz.log; tail -n 5 sz.log

*.h:
	make clean

${objs}: *.h

${prog}: ${objs} libjb/libjb.a
	${CC} ${CFLAGS} ${objs} libjb/libjb.a -o $@ ${LDFLAGS}

clean:
	rm -f ${prog} *.o
	cd libjb && make clean

install:
	install -d ${installdir}/bin
	install -s xstatus ${installdir}/bin

