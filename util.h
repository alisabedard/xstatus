// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_UTIL_H
#define XSTATUS_UTIL_H
#include <stdbool.h>
#include "libjb/xcb.h"
#include "xdata.h"
xcb_gc_t xstatus_get_gc(struct XData * restrict X, char * fg, char * bg);
#if defined(XSTATUS_USE_BATTERY_BAR) || defined(XSTATUS_USE_TEMPERATURE)
uint32_t xstatus_system_value(const char *filename);
#else
#define xstatus_system_value(f)
#endif
pixel_t xstatus_get_pixel(struct XData * restrict X,
	const char * restrict color);
#endif//XSTATUS_UTIL_H
