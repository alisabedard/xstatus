objs=${prog}.o util.o widget.o main.o font.o xdata.o button.o
objs+=clock.o load.o status_file.o
include config.mk
cflags+=-std=c11
cflags+=-D_XOPEN_SOURCE=700
cflags+=-D_DEFAULT_SOURCE
cflags+=-D_BSD_SOURCE
ldflags+=-Llibjb
ldflags+=-lxcb -ljb
ldlags+=${LDFLAGS}
cflags+=${CFLAGS}
PREFIX=/usr
prog=xstatus
installdir=${DESTDIR}${PREFIX}
${prog}: ${objs}
	${CC} ${cflags} ${ldflags} ${objs} -o $@
include depend.mk
clean:
	rm -f ${prog} *.o
install:
	install -d ${installdir}/bin
	install -s xstatus ${installdir}/bin
depend:
	${CC} -E -MM *.c > depend.mk
#EOF
