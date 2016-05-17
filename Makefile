CFLAGS=-Os
#CFLAGS+=-DDEBUG
#CFLAGS+=-ggdb
#CFLAGS+=-Werror 
#CFLAGS=-Os
#CC=clang

CFLAGS+=-Wall -Wextra
CFLAGS+=-std=c11
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=-D_BSD_SOURCE
CFLAGS+=-D_DEFAULT_SOURCE
CFLAGS+=-I/usr/X11R7/include
LDFLAGS+=-L/usr/X11R7/lib -lX11 -Wl,-R/usr/X11R7/lib
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

all: ${prog}
	ls -l ${prog} >> sz.log; tail sz.log

*.h:
	make clean

${objs}: *.h

${prog}: ${objs} 
	${CC} ${CFLAGS} ${objs} -o $@ ${LDFLAGS}

clean:
	rm -f ${prog} *.o

install:
	install -s xstatus ${installdir}/bin

