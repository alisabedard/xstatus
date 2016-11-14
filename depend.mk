battery.o: battery.c battery.h xdata.h config.h font.h libjb/JBDim.h \
 util.h libjb/xcb.h xstatus.h
button.o: button.c button.h widget.h libjb/xcb.h config.h font.h \
 libjb/JBDim.h xdata.h
clock.o: clock.c clock.h xdata.h config.h font.h libjb/JBDim.h
font.o: font.c font.h libjb/JBDim.h libjb/log.h libjb/xcb.h
load.o: load.c load.h xdata.h config.h font.h libjb/JBDim.h
main.o: main.c xstatus.h config.h
status_file.o: status_file.c status_file.h config.h font.h libjb/JBDim.h \
 util.h libjb/xcb.h xdata.h
temperature.o: temperature.c temperature.h config.h font.h libjb/JBDim.h \
 util.h libjb/xcb.h xdata.h
util.o: util.c util.h libjb/xcb.h xdata.h font.h libjb/JBDim.h
widget.o: widget.c widget.h libjb/xcb.h xdata.h
xdata.o: xdata.c xdata.h
xstatus.o: xstatus.c xstatus.h battery.h xdata.h button.h widget.h \
 libjb/xcb.h clock.h config.h font.h libjb/JBDim.h load.h status_file.h \
 temperature.h util.h
