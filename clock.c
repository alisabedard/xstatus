// Copyright 2016, Jeffrey E. Bedard
#include "clock.h"
#include "config.h"
#include "font.h"
#include "libjb/xcb.h"
__attribute__((hot))
uint16_t xstatus_draw_clock(xcb_connection_t * xc)
{
	char buf[30];
	const size_t sz = strftime(buf, sizeof buf,
		XSTATUS_TIME_FORMAT, localtime(&(time_t){time(NULL)}));
	const struct JBDim f = xstatus_get_font_size();
	const uint16_t offset = xstatus_get_screen(xc)->width_in_pixels
		- f.w * sz;
	xcb_image_text_8(xc, sz, xstatus_get_window(xc),
		xstatus_get_gc(xc),offset, f.h, buf);
	return offset;
}
