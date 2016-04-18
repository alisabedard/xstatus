#ifndef XS_UTIL_H
#define XS_UTIL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include "xdata.h"

typedef uint32_t Pixel;

uint16_t font_y(XFontStruct * restrict f);

uint16_t string_width(XFontStruct * restrict f, const size_t sz);

GC colorgc(XData * restrict X, const char * restrict color);

Display * get_display();

uint32_t sysval(const char *filename);

bool XNextEventTimed(Display * restrict dsp, XEvent * restrict event_return,
	const uint8_t delay);

Pixel pixel(Display * restrict d, const char * restrict color);

#endif//XS_UTIL_H
