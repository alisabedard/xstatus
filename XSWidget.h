// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_XSWIDGET_H
#define XSTATUS_XSWIDGET_H
#include <xcb/xcb.h>
struct XSWidget {
	xcb_connection_t * connection;
	xcb_window_t window;
	xcb_gcontext_t foreground, background;
	int16_t x, y;
	uint16_t width, height;
};
#endif//!XSTATUS_XSWIDGET_H
