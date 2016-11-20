battery.o: battery.c battery.h xdata.h config.h font.h libjb/JBDim.h \
 libjb/log.h libjb/util.h util.h libjb/xcb.h xstatus.h
button.o: button.c button.h config.h font.h libjb/JBDim.h libjb/xcb.h \
 xdata.h
clock.o: clock.c clock.h xdata.h config.h font.h libjb/JBDim.h \
 libjb/log.h libjb/xcb.h
font.o: font.c font.h libjb/JBDim.h libjb/log.h libjb/xcb.h
load.o: load.c load.h xdata.h config.h font.h libjb/JBDim.h
main.o: main.c xstatus.h config.h
status_file.o: status_file.c status_file.h config.h font.h libjb/JBDim.h \
 libjb/file.h libjb/util.h xdata.h
temperature.o: temperature.c temperature.h config.h font.h libjb/JBDim.h \
 util.h libjb/xcb.h xdata.h
toolbar.o: toolbar.c toolbar.h button.h config.h libjb/log.h
util.o: util.c util.h libjb/xcb.h xdata.h font.h libjb/JBDim.h \
 libjb/file.h
window.o: window.c window.h config.h libjb/xcb.h xdata.h
xdata.o: xdata.c xdata.h libjb/xcb.h
xstatus.o: xstatus.c xstatus.h battery.h xdata.h button.h clock.h \
 config.h font.h libjb/JBDim.h libjb/log.h libjb/xcb.h load.h \
 status_file.h temperature.h toolbar.h util.h window.h
