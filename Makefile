CFLAGS+=-Wall -Wextra -ggdb
CFLAGS+=-std=c11
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=-D_DEFAULT_SOURCE
CFLAGS+=-D_BSD_SOURCE -D__BSD_VISIBLE

LIBS+=-lxcb

# For FreeBSD build:
CFLAGS+=-I/usr/local/include
LDFLAGS+=-L/usr/local/lib

# For OpenBSD build:
CFLAGS+=-I/usr/X11R6/include
LDFLAGS+=-L/usr/X11R6/lib -Wl,-R/usr/X11R6/lib

# For NetBSD build:
CFLAGS+=-I/usr/pkg/include
CFLAGS+=-I/usr/X11R7/include
CFLAGS+=-L/usr/pkg/lib -Wl,-R/usr/pkg/lib
LDFLAGS+=-L/usr/X11R7/lib -Wl,-R/usr/X11R7/lib

CFLAGS+=${LIBS}

PREFIX=/usr
prog=xstatus
objs=${prog}.o util.o widget.o main.o

# Uncomment to enable buttons:
objs+=button.o
CFLAGS+=-DUSE_BUTTONS

# Uncomment to enable system load display:
objs+=load.o
CFLAGS+=-DUSE_LOAD

# Uncomment to enable temperature display:
objs+=temperature.o
CFLAGS+=-DUSE_TEMP

# Uncomment for status file display:
#objs+=status_file.o
#CFLAGS+=-DUSE_STATUS

# Uncomment for clock display:
objs+=clock.o
CFLAGS+=-DUSE_CLOCK

# Uncomment for battery status display:
objs+=battery.o
CFLAGS+=-DUSE_BATTERY

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

