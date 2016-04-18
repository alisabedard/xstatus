#ifndef XS_XDATA_H
#define XS_XDATA_H

#include <X11/Xlib.h>

typedef struct XData {
	Window w;
	GC gc;
	Display * d;
	XFontStruct * font;
} XData;

#endif//!XS_XDATA_H
