// Copyright 2016, Jeffrey E. Bedard

#ifndef XS_XDATA_H
#define XS_XDATA_H

#include <xcb/xcb.h>

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
