// Copyright 2017, Jeffrey E. Bedard
#ifndef XSTATUS_UTIL_H
#define XSTATUS_UTIL_H
#include <stdbool.h>
#include "libjb/xcb.h"
#include "xdata.h"
void xstatus_create_gc(xcb_connection_t * xc, const xcb_gcontext_t gc,
	const xcb_window_t win, const char * restrict fg,
	const char * restrict bg);
// returns -1 on error
int xstatus_system_value(const char * filename) __attribute__((nonnull(1)));
#endif//XSTATUS_UTIL_H
