// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_WIDGET_H
#define XSTATUS_WIDGET_H
#include "util.h"
struct XStatusWidget {
	struct XData * X; // X->w is the parent window.
	xcb_rectangle_t geometry;
	xcb_window_t window;
};
struct XStatusWidget * xstatus_get_widget(
	struct XStatusWidget * restrict w,
	struct XData * restrict X,
	xcb_rectangle_t * restrict geometry, const pixel_t bg,
	const uint32_t events);
#endif//!XSTATUS_WIDGET_H
