#ifndef XS_XDATA_H
#define XS_XDATA_H

#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>

typedef struct XData {
	xcb_connection_t * xcb;
	xcb_screen_t * screen;
	xcb_gc_t gc;
	xcb_window_t w;
	xcb_rectangle_t sz;
	xcb_font_t font;
	uint8_t font_width;
	uint8_t font_height;
} XData;

#endif//!XS_XDATA_H
