// Copyright 2016, Jeffrey E. Bedard
#ifndef XS_XDATA_H
#define XS_XDATA_H
#include "libjb/size.h"
#include <xcb/xcb.h>
struct XData {
	xcb_connection_t * xcb;
	xcb_screen_t * screen;
	xcb_gc_t gc;
	xcb_window_t w;
	xcb_rectangle_t sz;
};
#endif//!XS_XDATA_H
