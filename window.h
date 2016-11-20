// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_WINDOW_H
#define XSTATUS_WINDOW_H
#include <xcb/xcb.h>
void xstatus_create_window(xcb_connection_t * restrict xc)
	__attribute__((nonnull));
#endif//!XSTATUS_WINDOW_H
