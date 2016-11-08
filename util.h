// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_UTIL_H
#define XSTATUS_UTIL_H
#include <stdbool.h>
#include "libjb/xcb.h"
#include "xdata.h"
void xstatus_create_gc(xcb_connection_t * xc, const xcb_gc_t gc,
	const xcb_window_t win, char * restrict fg, char * restrict bg);
#if defined(XSTATUS_USE_BATTERY_BAR) || defined(XSTATUS_USE_TEMPERATURE)
uint32_t xstatus_system_value(const char *filename);
#else
#define xstatus_system_value(f)
#endif
pixel_t xstatus_get_pixel(xcb_connection_t * xc,
	const char * restrict color);
#endif//XSTATUS_UTIL_H
