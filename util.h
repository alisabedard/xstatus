// Copyright 2016, Jeffrey E. Bedard

#ifndef XS_UTIL_H
#define XS_UTIL_H

#include <stdbool.h>
#include "xdata.h"

typedef uint32_t Pixel;

xcb_gc_t xcbgc(XData * restrict X, char * fg, char * bg);

#if defined(USE_BATTERY) || defined(USE_TEMP)
uint32_t sysval(const char *filename);
#else
#define sysval(f)
#endif

bool next_event_timed(XData * restrict X,
	xcb_generic_event_t ** restrict e,
	const uint8_t delay);

Pixel pixel(XData * restrict X, const char * restrict color);

#endif//XS_UTIL_H
