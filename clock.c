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
	const uint16_t offset = X->screen->width_in_pixels
		- X->font_width * sz;
	xcb_image_text_8(X->xcb, sz, X->w, X->gc, offset,
		X->font_height, buf);

	return offset;
}

