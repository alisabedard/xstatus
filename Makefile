
CFLAGS+=-std=c11
LDFLAGS+=-lxcb

PREFIX=/usr
prog=xstatus
objs=${prog}.o util.o widget.o main.o
include config.mk

## Uncomment to enable buttons:
#objs+=button.o
#CFLAGS+=-DUSE_BUTTONS
#
## Uncomment to enable system load display:
#objs+=load.o
#CFLAGS+=-DUSE_LOAD
#
## Uncomment to enable temperature display:
#objs+=temperature.o
#CFLAGS+=-DUSE_TEMP
#
## Uncomment for status file display:
##objs+=status_file.o
##CFLAGS+=-DUSE_STATUS
#
## Uncomment for clock display:
#objs+=clock.o
#CFLAGS+=-DUSE_CLOCK
#
## Uncomment for battery status display:
#objs+=battery.o
#CFLAGS+=-DUSE_BATTERY
#
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

