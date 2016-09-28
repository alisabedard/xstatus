CFLAGS+=-std=c99
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=-D_DEFAULT_SOURCE
CFLAGS+=-D_BSD_SOURCE

# Porting:
# For OpenBSD:
CC=clang
CFLAGS+=-I/usr/X11R6/include
LDFLAGS+=-L/usr/X11R6/lib -Wl,-R/usr/X11R6/lib

# For NetBSD:
#CFLAGS+=-I/usr/X11R7/include
#LDFLAGS+=-L/usr/X11R7/lib -Wl,-R/usr/X11R7/lib

# Libs:
LDFLAGS+=-Llibjb
LDFLAGS+=-lxcb -ljb
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

