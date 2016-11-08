// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_WIDGET_H
#define XSTATUS_WIDGET_H
#include "libjb/xcb.h"
struct XStatusWidget {
	xcb_rectangle_t geometry;
	xcb_window_t window;
};
void xstatus_create_widget_window(xcb_connection_t * restrict xc,
	const xcb_window_t window, xcb_rectangle_t * restrict geo,
	const pixel_t bg, const uint32_t events);
struct XStatusWidget * xstatus_get_widget(
	xcb_connection_t * restrict xc,
	struct XStatusWidget * restrict w,
	xcb_rectangle_t * restrict geometry, const pixel_t bg,
	const uint32_t events);
#endif//!XSTATUS_WIDGET_H
