// Copyright 2016, Jeffrey E. Bedard
#ifndef XS_XDATA_H
#define XS_XDATA_H
#include "libjb/size.h"
#include <xcb/xcb.h>
struct XData {
	xcb_connection_t * xcb;
	xcb_window_t w;
};
xcb_colormap_t xstatus_get_colormap(xcb_connection_t * xc);
xcb_gc_t xstatus_get_button_gc(xcb_connection_t * xc);
xcb_gc_t xstatus_get_gc(xcb_connection_t * xc);
xcb_screen_t * xstatus_get_screen(xcb_connection_t * xc);
#endif//!XS_XDATA_H
