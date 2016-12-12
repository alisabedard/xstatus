objs=${exe}.o util.o main.o font.o xdata.o button.o
objs+=clock.o load.o status_file.o window.o toolbar.o
objs+=battery.o temperature.o
include config.mk
cflags+=-std=c11
cflags+=-D_XOPEN_SOURCE=700
cflags+=-D_DEFAULT_SOURCE
cflags+=-D_BSD_SOURCE
ldflags+=-lxcb -lxcb-cursor
ldlags+=${LDFLAGS}
cflags+=${CFLAGS}
PREFIX=/usr
exe=xstatus
installdir=${DESTDIR}${PREFIX}
${exe}: libjb/libjb.a ${objs}
	${CC} ${cflags} ${ldflags} ${objs} ${static} -o $@
	strip -s -o xstatus_stripped xstatus
	ls -l xstatus_stripped >> sz.log
	rm -f xstatus_stripped
	tail sz.log
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
cppcheck:
	cppcheck --enable=all --inconclusive --std=c11 \
                . 2> cppcheck.log
	echo 'Results written to cppcheck.log'
#EOF
