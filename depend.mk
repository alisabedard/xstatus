battery.o: battery.c battery.h xdata.h libjb/size.h config.h clock.h \
 xstatus.h util.h libjb/xcb.h libjb/log.h libjb/util.h
button.o: button.c button.h widget.h util.h libjb/xcb.h xdata.h \
 libjb/size.h config.h
clock.o: clock.c clock.h config.h libjb/xcb.h
load.o: load.c load.h config.h
main.o: main.c xstatus.h config.h
status_file.o: status_file.c status_file.h config.h libjb/util.h util.h \
 libjb/xcb.h xdata.h libjb/size.h
temperature.o: temperature.c temperature.h config.h util.h libjb/xcb.h \
 xdata.h libjb/size.h
util.o: util.c util.h libjb/xcb.h xdata.h libjb/size.h libjb/file.h \
 libjb/util.h
widget.o: widget.c widget.h util.h libjb/xcb.h xdata.h libjb/size.h
xstatus.o: xstatus.c xstatus.h battery.h xdata.h libjb/size.h button.h \
 widget.h util.h libjb/xcb.h clock.h config.h libjb/class.h libjb/log.h \
 load.h status_file.h temperature.h
