#ifndef XS_XDATA_H
#define XS_XDATA_H

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

typedef struct XData {
	xcb_connection_t * xcb;
	xcb_screen_t * screen;
	Display * d;
	XFontStruct * font;
	GC gc;
	xcb_window_t w;
	xcb_rectangle_t sz;
} XData;

#endif//!XS_XDATA_H
