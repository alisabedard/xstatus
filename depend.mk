battery.o: battery.c battery.h libjb/JBDim.h XSTextWidget.h XSWidget.h \
 config.h font.h libjb/macros.h text_widget.h util.h libjb/xcb.h xdata.h
button.o: button.c button.h XSWidget.h libjb/JBDim.h XSTextWidget.h \
 config.h font.h libjb/xcb.h text_widget.h xdata.h
clock.o: clock.c clock.h xdata.h XSTextWidget.h config.h font.h \
 libjb/JBDim.h text_widget.h
font.o: font.c font.h libjb/JBDim.h libjb/xcb.h xdata.h
load.o: load.c load.h xdata.h XSTextWidget.h text_widget.h
main.o: main.c xstatus.h config.h
status_file.o: status_file.c status_file.h XSTextWidget.h libjb/file.h \
 config.h text_widget.h
temperature.o: temperature.c temperature.h XSTextWidget.h config.h font.h \
 libjb/JBDim.h text_widget.h util.h libjb/xcb.h xdata.h
text_widget.o: text_widget.c text_widget.h XSTextWidget.h config.h font.h \
 libjb/JBDim.h xdata.h
toolbar.o: toolbar.c toolbar.h button.h XSWidget.h libjb/JBDim.h config.h \
 libjb/log.h libjb/util.h
util.o: util.c util.h libjb/xcb.h xdata.h font.h libjb/JBDim.h \
 libjb/file.h
window.o: window.c window.h config.h libjb/xcb.h xdata.h
xdata.o: xdata.c xdata.h libjb/xcb.h
xstatus.o: xstatus.c xstatus.h battery.h libjb/JBDim.h clock.h xdata.h \
 config.h font.h libjb/log.h libjb/xcb.h load.h status_file.h \
 temperature.h toolbar.h util.h window.h
