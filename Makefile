#CFLAGS=-O0
#CFLAGS+=-DDEBUG
#CFLAGS+=-ggdb
#CFLAGS+=-Werror 
CFLAGS=-Os
#CC=clang

CFLAGS+=-Wall -W
CFLAGS+=-std=c11
CFLAGS+=-D_XOPEN_SOURCE=700
CFLAGS+=-D_BSD_SOURCE
CFLAGS+=-D_DEFAULT_SOURCE
CFLAGS+=-I/usr/X11R7/include
LDFLAGS+=-L/usr/X11R7/lib -lX11 -Wl,-R/usr/X11R7/lib
PREFIX=/usr
prog=xstatus
srcs=${prog}.c battery.c clock.c util.c button.c widget.c main.c
objs=${srcs:.c=.o}
installdir=${DESTDIR}${PREFIX}

all: ${prog}
	ls -l ${prog} >> sz.log; tail sz.log

*.h:
	make clean

${objs}: *.h

${prog}: ${objs} 
	${CC} ${CFLAGS} ${objs} -o $@ ${LDFLAGS}

clean:
	rm -f ${prog} ${objs}

install:
	install -s xstatus ${installdir}/bin
