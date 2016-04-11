#include <stdint.h>
#include <time.h>
#include <X11/Xlib.h>
#include "config.h"
#include "util.h"

uint16_t xstatus_clock_x;

__attribute__((hot))
void draw_clock(Display * restrict d, const Window w, const GC gc)
{
	time_t t=time(NULL);
	char buf[30];
	size_t sz = strftime(buf, sizeof buf, TIMEFMT, localtime(&t));
	xstatus_clock_x = DisplayWidth(d, 0) - string_width(sz);
	XDrawString(d, w, gc, xstatus_clock_x, font_y(), buf, sz);
}

