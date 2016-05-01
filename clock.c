#include <stdint.h>
#include <time.h>
#include <X11/Xlib.h>
#include "config.h"
#include "util.h"
#include "xdata.h"

__attribute__((hot))
uint16_t draw_clock(XData * restrict X)
{
	char buf[30];
	const size_t sz = strftime(buf, sizeof buf, 
		TIMEFMT, localtime(&(time_t){time(NULL)}));
	const uint16_t offset = DisplayWidth(X->d, 0) 
		- XTextWidth(X->font, buf, sz);
	XDrawString(X->d, X->w, X->gc, offset, font_y(X->font), buf, sz);

	return offset;
}

