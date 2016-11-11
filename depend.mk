battery.o: battery.c battery.h xdata.h config.h font.h util.h xstatus.h
button.o: button.c button.h widget.h config.h font.h xdata.h
clock.o: clock.c clock.h xdata.h config.h font.h
font.o: font.c font.h
load.o: load.c load.h xdata.h config.h font.h
main.o: main.c xstatus.h config.h
status_file.o: status_file.c status_file.h config.h font.h util.h xdata.h
temperature.o: temperature.c temperature.h config.h font.h util.h xdata.h
util.o: util.c util.h xdata.h font.h
widget.o: widget.c widget.h xdata.h
xdata.o: xdata.c xdata.h
xstatus.o: xstatus.c xstatus.h battery.h xdata.h button.h widget.h \
 clock.h config.h font.h load.h status_file.h temperature.h util.h
