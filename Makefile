CFLAGS+=-Wall -Wextra -ggdb
CFLAGS+=-std=c11
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=-D_DEFAULT_SOURCE
CFLAGS+=-I/usr/X11R7/include
# For NetBSD build:
LDFLAGS+=-L/usr/X11R6/lib -Wl,-R/usr/X11R6/lib
LDFLAGS+=-L/usr/X11R7/lib -Wl,-R/usr/X11R7/lib
LDFLAGS+=-Llibjb
# Libs:
LDFLAGS+=-lxcb -ljb
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
	cd libjb && make
	make ${prog}
	ls -l ${prog} >> sz.log; tail -n 5 sz.log

*.h:
	make clean

${objs}: *.h

${prog}: ${objs} libjb/libjb.a
	${CC} ${CFLAGS} ${objs} -o $@ ${LDFLAGS}

clean:
	rm -f ${prog} *.o
	cd libjb && make clean

install:
	install -d ${installdir}/bin
	install -s xstatus ${installdir}/bin

