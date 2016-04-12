#ifndef XS_CLOCK_H
#define XS_CLOCK_H

#include <stdint.h>
#include <X11/Xlib.h>

extern uint16_t xstatus_clock_x;

void draw_clock(Display * restrict d, const Window w, const GC gc);

#endif//!XS_CLOCK_H
