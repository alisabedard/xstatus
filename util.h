// Copyright 2016, Jeffrey E. Bedard
#ifndef XSTATUS_UTIL_H
#define XSTATUS_UTIL_H
#include <stdbool.h>
#include "xdata.h"
typedef uint32_t Pixel;
xcb_gc_t xcbgc(struct XData * restrict X, char * fg, char * bg);
#if defined(XSTATUS_USE_BATTERY_BAR) || defined(XSTATUS_USE_TEMPERATURE)
uint32_t sysval(const char *filename);
#else
#define sysval(f)
#endif
Pixel pixel(struct XData * restrict X, const char * restrict color);
#endif//XSTATUS_UTIL_H
