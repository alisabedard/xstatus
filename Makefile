objs=${exe}.o util.o main.o font.o xdata.o button.o
objs+=clock.o load.o status_file.o
include config.mk
cflags+=-std=c11
cflags+=-D_XOPEN_SOURCE=700
cflags+=-D_DEFAULT_SOURCE
cflags+=-D_BSD_SOURCE
ldflags+=-lxcb
ldlags+=${LDFLAGS}
cflags+=${CFLAGS}
PREFIX=/usr
exe=xstatus
installdir=${DESTDIR}${PREFIX}
${exe}: libjb/libjb.a ${objs}
	${CC} ${cflags} ${ldflags} ${objs} ${static} -o $@
libjb/libjb.a:
	cd libjb && ${MAKE} libjb.a
include depend.mk
clean:
	cd libjb && ${MAKE} clean
	rm -f ${exe} *.o
install:
#	cd libjb && ${MAKE} install
	install -d ${installdir}/bin
	install -s xstatus ${installdir}/bin
depend:
	${CC} -E -MM *.c > depend.mk
#EOF
