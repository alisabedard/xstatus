#ifndef XS_UTIL_H
#define XS_UTIL_H

#include <stdbool.h>
#include <X11/Xlib.h>
#include <stdio.h>

typedef uint32_t Pixel;

uint16_t font_y(void);

uint16_t string_width(const size_t sz);

GC colorgc(Display * restrict d, const Window w, const char * restrict color);

Display * get_display();

uint32_t sysval(const char *filename);

bool XNextEventTimed(Display * restrict dsp, XEvent * restrict event_return);

Pixel pixel(Display * restrict d, const char * restrict color);

#endif//XS_UTIL_H
