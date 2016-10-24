// Copyright 2016, Jeffrey E. Bedard
#include "clock.h"
#include "config.h"
#include "libjb/xcb.h"
__attribute__((hot))
uint16_t xstatus_draw_clock(struct XData * restrict X)
{
	char buf[30];
	const size_t sz = strftime(buf, sizeof buf,
		XSTATUS_TIME_FORMAT, localtime(&(time_t){time(NULL)}));
	const struct JBDim f = X->font_size;
	const uint16_t offset = X->screen->width_in_pixels - f.w * sz;
	xcb_image_text_8(X->xcb, sz, X->w, X->gc, offset, f.h, buf);
	return offset;
}
